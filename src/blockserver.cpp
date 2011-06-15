#include <limits.h>

#include "blockserver.h"

#include "blueblock.h"
#include "purpleblock.h"
#include "redblock.h"
#include "yellowblock.h"

BlockServer::BlockServer(s32 playerCount, s32 blockColourCount) {
	_blockColourCount = blockColourCount;
	_playerCount = playerCount;

	_playerBlockListIndices = new s32[playerCount];

	clear();
}

BlockServer::~BlockServer() {
	delete[] _playerBlockListIndices;
}

void BlockServer::clear() {
	for (s32 i = 0; i < _playerCount; ++i) {
		_playerBlockListIndices[i] = 0;
	}
	
	_blockList.clear();
}

void BlockServer::getNextBlocks(s32 player, BlockBase** block1, BlockBase** block2) {
	s32 index = _playerBlockListIndices[player]++;

	// If the player is requesting a block past the end of the block list,
	// we need to append a new pair before we can return it
	if (index == _blockList.size()) {
		addRandomBlockPair();
	}

	*block1 = newBlockFromType(_blockList[index].block1);
	*block2 = newBlockFromType(_blockList[index].block2);

	// We can try to expire any old blocks in the list now
	expireUsedBlocks();
}

void BlockServer::addRandomBlockPair() {
	BlockPair pair;
	pair.block1 = getRandomBlockType();
	pair.block2 = getRandomBlockType();

	_blockList.push_back(pair);
}

BlockBase* BlockServer::newBlockFromType(BlockServer::BlockType type) const {
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

void BlockServer::expireUsedBlocks() {
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
}

BlockServer::BlockType BlockServer::getRandomBlockType() const {
	return (BlockServer::BlockType)(rand() % _blockColourCount);
}
