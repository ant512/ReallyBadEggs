#ifndef _BLOCK_SERVER_H_
#define _BLOCK_SERVER_H_

#include <limits.h>
#include <nds.h>
#include <woopsiarray.h>

#include "blockbase.h"
#include "blueblock.h"
#include "purpleblock.h"
#include "redblock.h"
#include "yellowblock.h"

class BlockServer {
public:
	BlockServer(s32 playerCount, s32 blockColourCount) {
		_blockColourCount = blockColourCount;
		_playerCount = playerCount;

		_playerBlockListIndices = new s32[playerCount];

		for (s32 i = 0; i < playerCount; ++i) {
			_playerBlockListIndices[i] = 0;
		}
	};

	~BlockServer() {
		delete[] _playerBlockListIndices;
	};

	void getNextBlocks(s32 player, BlockBase** block1, BlockBase** block2) {
		s32 index = _playerBlockListIndices[player]++;

		// If the player is requesting a block past the end of the block list,
		// we need to append a new pair before we can return it
		if (index == _blockList.size()) {
			addRandomBlockPair();
		}

		*block1 = getBlockFromType(_blockList[index].block1);
		*block2 = getBlockFromType(_blockList[index].block2);

		// We can try to expire any old blocks in the list now
		expireUsedBlocks();
	};

private:

	enum BlockType {
		BLOCK_RED = 0,
		BLOCK_BLUE = 1,
		BLOCK_YELLOW = 2,
		BLOCK_PURPLE = 3,
		BLOCK_GREEN = 4,
		BLOCK_ORANGE = 5
	};

	typedef struct {
		BlockType block1;
		BlockType block2;
	} BlockPair;

	WoopsiArray<BlockPair> _blockList;
	s32* _playerBlockListIndices;
	s32 _blockColourCount;
	s32 _playerCount;

	void addRandomBlockPair() {
		BlockPair pair;
		pair.block1 = getRandomBlockType();
		pair.block2 = getRandomBlockType();

		_blockList.push_back(pair);
	};

	BlockBase* getBlockFromType(BlockType type) const {
		switch (type) {
			case BLOCK_RED:
				return new RedBlock();
			case BLOCK_PURPLE:
				return new PurpleBlock();
			case BLOCK_YELLOW:
				return new YellowBlock();
			case BLOCK_BLUE:
				return new BlueBlock();
			case BLOCK_GREEN:
				return new RedBlock();
			case BLOCK_ORANGE:
				return new RedBlock();
		}

		// Included to silence compiler warning
		return new RedBlock();
	}

	void expireUsedBlocks() {
		s32 minimumBlock = INT_MAX;

		// Locate the earliest-used block in the list
		for (s32 i = 0; i < _playerCount; ++i) {
			if (_playerBlockListIndices[i] < minimumBlock) minimumBlock = _playerBlockListIndices[i];
		}

		// Reduce the indices of all players as we are going to trash everything
		// before the earliest-used block
		for (s32 i = 0; i < _playerCount; ++i) {
			_playerBlockListIndices[i] -= minimumBlock;
		}

		// Trash the unused blocks from the start of the array
		while (minimumBlock > 0) {
			_blockList.erase(0);
			--minimumBlock;
		}
	};

	BlockType getRandomBlockType() const {
		return (BlockType)(rand() % _blockColourCount);
	};
};

#endif
