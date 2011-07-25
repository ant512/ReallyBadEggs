#include <limits.h>

#include "blockfactory.h"

#include "blueblock.h"
#include "greenblock.h"
#include "orangeblock.h"
#include "purpleblock.h"
#include "redblock.h"
#include "yellowblock.h"

BlockFactory::BlockFactory(s32 playerCount, s32 blockColourCount) {
	_blockColourCount = blockColourCount;
	_playerCount = playerCount;

	_playerBlockListIndices = new s32[playerCount];

	clear();
}

BlockFactory::~BlockFactory() {
	delete[] _playerBlockListIndices;
}

void BlockFactory::clear() {
	for (s32 i = 0; i < _playerCount; ++i) {
		_playerBlockListIndices[i] = 0;
	}
	
	_blockList.clear();
}

BlockBase* BlockFactory::newBlock(s32 player) {
	s32 index = _playerBlockListIndices[player]++;

	// If the player is requesting a block past the end of the block list,
	// we need to append a new block before we can return it
	if (index == _blockList.size()) {
		addRandomBlock();
	}

	BlockBase* block = newBlockFromType(_blockList[index]);

	// We can try to expire any old blocks in the list now
	expireUsedBlocks();

	return block;
}

void BlockFactory::addRandomBlock() {
	_blockList.push_back(getRandomBlockType());
}

BlockBase* BlockFactory::newBlockFromType(BlockFactory::BlockType type) const {
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
			return new GreenBlock();
		case BLOCK_ORANGE:
			return new OrangeBlock();
	}

	// Included to silence compiler warning
	return new RedBlock();
}

void BlockFactory::expireUsedBlocks() {
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

BlockFactory::BlockType BlockFactory::getRandomBlockType() const {
	return (BlockFactory::BlockType)(rand() % _blockColourCount);
}
