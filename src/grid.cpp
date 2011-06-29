#include "grid.h"
#include "garbageblock.h"
#include "redblock.h"
#include "purpleblock.h"
#include "yellowblock.h"
#include "blueblock.h"
#include "soundplayer.h"

Grid::Grid(s32 startingHeight, s32 playerNumber) {
	_data = new BlockBase*[GRID_WIDTH * GRID_HEIGHT];

	_liveBlocks = new Point[2];
	_hasLiveBlocks = false;

	_playerNumber = playerNumber;

	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		_data[i] = NULL;
	}

	// Add rows of garbage
	if (startingHeight > GRID_HEIGHT) startingHeight = GRID_HEIGHT;

	for (s32 row = 0; row < startingHeight; ++row) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {
			s32 y = GRID_HEIGHT - 1 - row;

			setBlockAt(x, y, new GarbageBlock());
		}
	}

	_columnOffsets = new s32[GRID_WIDTH];

	for (s32 i = 0; i < GRID_WIDTH; ++i) {
		_columnOffsets[i] = 0;
	}
}

Grid::~Grid() {
	clear();
	delete[] _data;
	delete[] _liveBlocks;
	delete[] _columnOffsets;
}

void Grid::clear() {

	// Delete everything in the grid
	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		if (_data[i] != NULL) {
			delete _data[i];
			_data[i] = NULL;
		}
	}

	for (s32 i = 0; i < GRID_WIDTH; ++i) {
		_columnOffsets[i] = 0;
	}
}

BlockBase* Grid::getBlockAt(s32 x, s32 y) const {
	if (!isValidCoordinate(x, y)) return 0;

	return _data[x + (y * GRID_WIDTH)];
}

void Grid::setBlockAt(s32 x, s32 y, BlockBase* block) {
	if (!isValidCoordinate(x, y)) return;

	s32 index = x + (y * GRID_WIDTH);

	if (_data[index] != NULL) {
		delete _data[index];
	}

	_data[index] = block;
}

void Grid::moveBlock(s32 srcX, s32 srcY, s32 destX, s32 destY) {
	if (!isValidCoordinate(srcX, srcY)) return;
	if (!isValidCoordinate(destX, destY)) return;
	if (srcX == destX && srcY == destY) return;

	s32 srcIndex = srcX + (srcY * GRID_WIDTH);
	s32 destIndex = destX + (destY * GRID_WIDTH);

	if (_data[destIndex] != NULL) {
		delete _data[destIndex];
	}

	_data[destIndex] = _data[srcIndex];
	_data[srcIndex] = NULL;
}

bool Grid::isValidCoordinate(s32 x, s32 y) const {
	if (x < 0) return false;
	if (x >= GRID_WIDTH) return false;
	if (y < 0) return false;
	if (y >= GRID_HEIGHT) return false;

	return true;
}

bool Grid::explodeChains(s32& score, s32& chainCount, s32& blocks) {

	WoopsiArray<WoopsiArray<Point>*> chains;
	
	score = 0;
	blocks = 0;

	getChains(chains);

	chainCount = chains.size();

	for (s32 i = 0; i < chains.size(); ++i) {

		score += BLOCK_EXPLODE_SCORE * chains[i]->size() * (chains[i]->size() - CHAIN_LENGTH + 1);
		blocks += chains[i]->size();

		for (s32 j = 0; j < chains[i]->size(); ++j) {

			Point& point = chains[i]->at(j);

			getBlockAt(point.x, point.y)->explode();

			// Remove any adjacent garbage

			BlockBase* garbage = getBlockAt(point.x - 1, point.y);
			if (garbage != NULL && garbage->getColour() == GarbageBlock::COLOUR) {
				getBlockAt(point.x - 1, point.y)->explode();

				score += BLOCK_EXPLODE_SCORE * i;
			}

			garbage = getBlockAt(point.x + 1, point.y);
			if (garbage != NULL && garbage->getColour() == GarbageBlock::COLOUR) {
				getBlockAt(point.x + 1, point.y)->explode();

				score += BLOCK_EXPLODE_SCORE * i;
			}

			garbage = getBlockAt(point.x, point.y - 1);
			if (garbage != NULL && garbage->getColour() == GarbageBlock::COLOUR) {
				getBlockAt(point.x, point.y - 1)->explode();

				score += BLOCK_EXPLODE_SCORE * i;
			}

			garbage = getBlockAt(point.x, point.y + 1);
			if (garbage != NULL && garbage->getColour() == GarbageBlock::COLOUR) {
				getBlockAt(point.x, point.y + 1)->explode();

				score += BLOCK_EXPLODE_SCORE * i;
			}
		}

		delete chains[i];
	}

	return score > 0;
}

void Grid::getChains(WoopsiArray<WoopsiArray<Point>*>& chains) const {

	// Array of bools remembers which blocks we've already examined so that we
	// don't check them again and get stuck in a loop
	bool* checkedData = new bool[GRID_WIDTH * GRID_HEIGHT];

	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		checkedData[i] = false;
	}

	for (s32 y = 0; y < GRID_HEIGHT; ++y) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {

			// Skip if block already checked
			if (checkedData[x + (y * GRID_WIDTH)]) continue;

			WoopsiArray<Point>* chain = new WoopsiArray<Point>();

			getChain(x, y, *chain, checkedData);

			// Only remember the chain if it has the minimum number of blocks in
			// it at least
			if (chain->size() >= CHAIN_LENGTH) {
				chains.push_back(chain);
			} else {
				delete chain;
			}
		}
	}

	delete[] checkedData;
}

void Grid::getLiveBlockPoints(Point& liveBlock1, Point& liveBlock2) const {
	liveBlock1 = _liveBlocks[0];
	liveBlock2 = _liveBlocks[1];
}

s32 Grid::getPotentialChainLength(s32 x, s32 y, BlockBase* block) const {
	
	bool* checkedData = new bool[GRID_WIDTH * GRID_HEIGHT];

	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		checkedData[i] = false;
	}

	checkedData[x + (y * GRID_WIDTH)] = true;

	WoopsiArray<Point> chain;

	BlockBase* gridBlock = getBlockAt(x - 1, y);
	if (gridBlock != NULL && gridBlock->getColour() == block->getColour()) {
		getChain(x - 1, y, chain, checkedData);
	}

	gridBlock = getBlockAt(x + 1, y);
	if (gridBlock != NULL && gridBlock->getColour() == block->getColour()) {
		getChain(x + 1, y, chain, checkedData);
	}

	gridBlock = getBlockAt(x, y - 1);
	if (gridBlock != NULL && gridBlock->getColour() == block->getColour()) {
		getChain(x, y - 1, chain, checkedData);
	}

	gridBlock = getBlockAt(x, y + 1);
	if (gridBlock != NULL && gridBlock->getColour() == block->getColour()) {
		getChain(x, y + 1, chain, checkedData);
	}

	delete[] checkedData;

	return chain.size() + 1;
}

void Grid::getChain(s32 x, s32 y, WoopsiArray<Point>& chain, bool* checkedData) const {

	// Stop if we've checked this block already
	if (checkedData[x + (y * GRID_WIDTH)]) return;

	s32 index = 0;

	// Add the start of the chain to the list of blocks that comprise the chain
	Point startPoint;
	startPoint.x = x;
	startPoint.y = y;

	chain.push_back(startPoint);

	// Ensure we don't check this block again
	checkedData[x + (y * GRID_WIDTH)] = true;

	// Check the blocks that surround every block in the chain to see if they
	// should be part of the chain.  If so, add them to the chain.
	while (index < chain.size()) {

		Point& point = chain[index];
		BlockBase* block = getBlockAt(point.x, point.y);

		if (block == NULL) return;

		// Check if the block on the left of this is part of the chain.  Ignore
		// the block if it has already been checked.
		if (!checkedData[point.x - 1 + (point.y * GRID_WIDTH)]) {

			if (block->hasLeftConnection()) {

				// Block is part of the chain so remember its co-ordinates
				Point adjacentPoint;
				adjacentPoint.x = point.x - 1;
				adjacentPoint.y = point.y;

				chain.push_back(adjacentPoint);

				// Now that we know this block is part of a chain we don't want
				// to check it again
				checkedData[point.x - 1 + (point.y * GRID_WIDTH)] = true;
			}
		}

		if (!checkedData[point.x + 1 + (point.y * GRID_WIDTH)]) {

			if (block->hasRightConnection()) {

				Point adjacentPoint;
				adjacentPoint.x = point.x + 1;
				adjacentPoint.y = point.y;

				chain.push_back(adjacentPoint);

				checkedData[point.x + 1 + (point.y * GRID_WIDTH)] = true;
			}
		}

		if (point.y - 1 >= 0 && !checkedData[point.x + ((point.y - 1) * GRID_WIDTH)]) {

			if (block->hasTopConnection()) {

				Point adjacentPoint;
				adjacentPoint.x = point.x;
				adjacentPoint.y = point.y - 1;

				chain.push_back(adjacentPoint);

				checkedData[point.x + ((point.y - 1) * GRID_WIDTH)] = true;
			}
		}

		if (point.y + 1 < GRID_HEIGHT && !checkedData[point.x + ((point.y + 1) * GRID_WIDTH)]) {

			if (block->hasBottomConnection()) {

				Point adjacentPoint;
				adjacentPoint.x = point.x;
				adjacentPoint.y = point.y + 1;

				chain.push_back(adjacentPoint);

				checkedData[point.x + ((point.y + 1) * GRID_WIDTH)] = true;
			}
		}

		index++;
	}
}

void Grid::dropLiveBlocks() {

	// Abort if we don't have live blocks to move
	if (!_hasLiveBlocks) return;

	bool hasLanded = false;

	// Check both live blocks for collisions before we try to drop them.  This
	// prevents us from getting into a situation in which one of the pair drops
	// and the other hits something
	for (s32 i = 0; i < 2; ++i) {

		// Check if block is at bottom of grid
		if (_liveBlocks[i].y + 1 >= GRID_HEIGHT) {
			_hasLiveBlocks = false;

			BlockBase* block = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
			block->land();

			hasLanded = true;
		} else {

			// Check if the block has landed on another
			BlockBase* blockBelow = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y + 1);

			if (blockBelow != NULL) {

				// Do not land if the block below is also falling
				if (!blockBelow->isFalling()) {
					_hasLiveBlocks = false;

					BlockBase* block = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
					block->land();

					hasLanded = true;
				}
			}
		}
	}

	if (_hasLiveBlocks) {

		// Blocks are still live - drop them to the next position.  Drop block
		// 1 first as when vertical 1 is always below
		for (s32 i = 1; i >= 0; --i) {

			BlockBase* liveBlock = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
			if (liveBlock->hasDroppedHalfBlock()) {
				moveBlock(_liveBlocks[i].x, _liveBlocks[i].y, _liveBlocks[i].x, _liveBlocks[i].y + 1);
				
				// Update the live block co-ordinates
				++_liveBlocks[i].y;
			}
			
			liveBlock->dropHalfBlock();
		}
	}

	if (hasLanded) {
		SoundPlayer::playLand(_playerNumber);
	}
}

bool Grid::dropBlocks() {

	// Do not allow blocks to drop if we have live blocks
	if (_hasLiveBlocks) return false;

	bool hasDropped = false;
	bool hasLanded = false;
	bool isGarbage = false;

	// Everything on the bottom row should have landed
	for (s32 x = 0; x < GRID_WIDTH; ++x) {
		BlockBase* block = getBlockAt(x, GRID_HEIGHT - 1);

		if (block != NULL && block->isFalling()) {

			// Shake the column
			if (block->getColour() == GarbageBlock::COLOUR) {
				_columnOffsets[x] = GARBAGE_LAND_OFFSET;

				isGarbage = true;
			}

			block->land();
			hasLanded = true;
		}
	}

	// Drop starts at the second row from the bottom of the grid as there's no
	// point in dropping the bottom row
	for (s32 y = GRID_HEIGHT - 2; y >= 0; --y) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {
			
			BlockBase* block = getBlockAt(x, y);

			// Ignore this block if it's empty
			if (block == NULL) continue;

			// Drop the current block if the block below is empty
			if (getBlockAt(x, y + 1) == NULL) {
				
				if (block->hasDroppedHalfBlock()) {
					moveBlock(x, y, x, y + 1);
				}
				
				block->dropHalfBlock();
				block->fall();

				hasDropped = true;
			} else if (block->isFalling()) {

				if (!getBlockAt(x, y + 1)->isFalling()) {
					block->land();
					hasLanded = true;

					// Shake the column
					if (block->getColour() == GarbageBlock::COLOUR) {
						_columnOffsets[x] = GARBAGE_LAND_OFFSET;

						isGarbage = true;
					}
				}
			}
		}
	}

	if (hasLanded) {
		if (isGarbage) {
			SoundPlayer::playGarbage(_playerNumber);
		} else {
			SoundPlayer::playLand(_playerNumber);
		}
	}

	return hasDropped;
}

void Grid::render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	s32 renderX = 0;
	s32 renderY = 0;

	for (s32 blockX = 0; blockX < GRID_WIDTH; ++blockX) {

		renderX = x + (blockX * BLOCK_SIZE);

		for (s32 blockY = GRID_HEIGHT - 1; blockY >= 0; --blockY) {
			
			renderY = y + (blockY * BLOCK_SIZE) + _columnOffsets[blockX];

			BlockBase* block = getBlockAt(blockX, blockY);

			if (block == NULL) {
				gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 0, 0));
			} else {
				if (block->hasDroppedHalfBlock()) {
					
					// Erase the half-block above
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE / 2, woopsiRGB(0, 0, 0));
					renderY += BLOCK_SIZE / 2;
				}

				block->render(renderX, renderY, gfx);
			}
		}
	}
}

bool Grid::hasLiveBlocks() const {
	return _hasLiveBlocks;
}

void Grid::moveLiveBlocksLeft() {
	if (!_hasLiveBlocks) return;

	bool canMove = true;

	// 0 block should always be on the left or at the top
	if (_liveBlocks[0].x == 0) canMove = false;

	// Check the block to the left
	if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != NULL) canMove = false;

	// If we've dropped half a block we also need to check the block left and
	// down one
	if (getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y)->hasDroppedHalfBlock()) {
		if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y + 1) != NULL) canMove = false;
	}

	// Check 1 block if it is below the 0 block
	if (_liveBlocks[0].x == _liveBlocks[1].x) {
		if (getBlockAt(_liveBlocks[1].x - 1, _liveBlocks[1].y) != NULL) canMove = false;

		// Check the block left and down one if we've dropped a half block
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y)->hasDroppedHalfBlock()) {
			if (getBlockAt(_liveBlocks[1].x - 1, _liveBlocks[1].y + 1) != NULL) canMove = false;
		}
	}

	if (canMove) {
		for (s32 i = 0; i < 2; ++i) {
			moveBlock(_liveBlocks[i].x, _liveBlocks[i].y, _liveBlocks[i].x - 1, _liveBlocks[i].y);
			--_liveBlocks[i].x;
		}

		SoundPlayer::playMove(_playerNumber);
	}
}

void Grid::moveLiveBlocksRight() {
	if (!_hasLiveBlocks) return;

	bool canMove = true;

	// 1 block should always be on the right or at the bottom
	if (_liveBlocks[1].x == GRID_WIDTH - 1) canMove = false;

	// Check the block to the right
	if (getBlockAt(_liveBlocks[1].x + 1, _liveBlocks[1].y) != NULL) canMove = false;

	// If we've dropped half a block we also need to check the block right and
	// down one
	if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y)->hasDroppedHalfBlock()) {
		if (getBlockAt(_liveBlocks[1].x + 1, _liveBlocks[1].y + 1) != NULL) canMove = false;
	}

	// Check 0 block if it is above the 1 block
	if (_liveBlocks[0].x == _liveBlocks[1].x) {
		if (getBlockAt(_liveBlocks[0].x + 1, _liveBlocks[0].y) != NULL) canMove = false;

		// Check the block right and down one if we've dropped a half block
		if (getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y)->hasDroppedHalfBlock()) {
			if (getBlockAt(_liveBlocks[0].x + 1, _liveBlocks[0].y + 1) != NULL) canMove = false;
		}
	}

	if (canMove) {
		for (s32 i = 1; i >= 0; --i) {
			moveBlock(_liveBlocks[i].x, _liveBlocks[i].y, _liveBlocks[i].x + 1, _liveBlocks[i].y);
			++_liveBlocks[i].x;
		}

		SoundPlayer::playMove(_playerNumber);
	}
}

void Grid::rotateLiveBlocksClockwise() {
	if (!_hasLiveBlocks) return;

	// Determine whether the blocks swap to a vertical or horizontal arrangement
	if (_liveBlocks[0].y == _liveBlocks[1].y) {

		// Swapping to vertical

		// Cannot swap if the blocks are at the bottom of the well or they have
		// dropped half a block
		if (_liveBlocks[0].y == GRID_HEIGHT - 1) return;
		if (_liveBlocks[0].y == GRID_HEIGHT - 2 && getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y)->hasDroppedHalfBlock()) return;


		// Cannot swap if the block below the block on the right is populated
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1) != NULL) return;

		// Cannot swap if the block 2 below the block on the right is populated
		// if we've dropped a half block
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y)->hasDroppedHalfBlock()) {
			if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 2) != NULL) return;
		}

		// Perform the rotation

		// Move the right block down one place
		moveBlock(_liveBlocks[1].x, _liveBlocks[1].y, _liveBlocks[1].x, _liveBlocks[1].y + 1);
		++_liveBlocks[1].y;

		// Move the left block right one place
		moveBlock(_liveBlocks[0].x, _liveBlocks[0].y, _liveBlocks[0].x + 1, _liveBlocks[0].y);
		++_liveBlocks[0].x;

	} else {

		// Swapping to horizontal

		// Cannot swap if the blocks are at the left edge of the well
		if (_liveBlocks[0].x == 0) return;

		// Cannot swap if the block to the left of the block at the top is populated
		if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != NULL) return;

		// Cannot swap if the block below the block on the left of the top block
		// is populated if we've dropped a half block
		if (getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y)->hasDroppedHalfBlock()) {
			if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y + 1) != NULL) return;
		}

		// Perform the rotation

		// Move the bottom block up and left
		moveBlock(_liveBlocks[1].x, _liveBlocks[1].y, _liveBlocks[0].x - 1, _liveBlocks[0].y);

		// 0 block should always be on the left
		_liveBlocks[1].x = _liveBlocks[0].x;
		_liveBlocks[1].y = _liveBlocks[0].y;

		--_liveBlocks[0].x;
	}

	SoundPlayer::playRotate(_playerNumber);
}

void Grid::rotateLiveBlocksAntiClockwise() {
	if (!_hasLiveBlocks) return;

	// Determine whether the blocks swap to a vertical or horizontal arrangement
	if (_liveBlocks[0].y == _liveBlocks[1].y) {

		// Swapping to vertical

		// Cannot swap if the blocks are at the bottom of the well or they have
		// dropped half a block
		if (_liveBlocks[0].y == GRID_HEIGHT - 1) return;
		if (_liveBlocks[0].y == GRID_HEIGHT - 2 && getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y)->hasDroppedHalfBlock()) return;

		// Cannot swap if the block below the block on the right is populated
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1) != NULL) return;

		// Cannot swap if the block 2 below the block on the right is populated
		// if we've dropped a half block
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y)->hasDroppedHalfBlock()) {
			if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 2) != NULL) return;
		}

		// Perform the rotation

		// Move the left block down and right
		moveBlock(_liveBlocks[0].x, _liveBlocks[0].y, _liveBlocks[1].x, _liveBlocks[1].y + 1);

		// 0 block should always be at the top
		_liveBlocks[0].x = _liveBlocks[1].x;
		_liveBlocks[0].y = _liveBlocks[1].y;
		++_liveBlocks[1].y;

	} else {

		// Swapping to horizontal

		// Cannot swap if the blocks are at the left edge of the well
		if (_liveBlocks[0].x == 0) return;

		// Cannot swap if the block to the left of the block at the top is populated
		if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != NULL) return;

		// Cannot swap if the block below the block on the left of the top block
		// is populated if we've dropped a half block
		if (getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y)->hasDroppedHalfBlock()) {
			if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y + 1) != NULL) return;
		}

		// Perform the rotation

		// Move the top block left
		moveBlock(_liveBlocks[0].x, _liveBlocks[0].y, _liveBlocks[0].x - 1, _liveBlocks[0].y);
		--_liveBlocks[0].x;

		// Move the bottom block up
		moveBlock(_liveBlocks[1].x, _liveBlocks[1].y, _liveBlocks[1].x, _liveBlocks[1].y - 1);
		--_liveBlocks[1].y;
	}

	SoundPlayer::playRotate(_playerNumber);
}

bool Grid::addLiveBlocks(BlockBase* block1, BlockBase* block2) {

	// Do not add more live blocks if we have blocks already.  However, return
	// true because we don't want to treat this as a special case; as far as
	// any other code is concerned it did its job - live blocks are in play
	if (_hasLiveBlocks) return true;

	// Cannot add live blocks if the grid positions already contain blocks
	if (getBlockAt(2, 0) != NULL) return false;
	if (getBlockAt(3, 0) != NULL) return false;

	// Live blocks always appear at the same co-ordinates
	setBlockAt(2, 0, block1);
	setBlockAt(3, 0, block2);

	_liveBlocks[0].x = 2;
	_liveBlocks[0].y = 0;

	_liveBlocks[1].x = 3;
	_liveBlocks[1].y = 0;

	_hasLiveBlocks = true;

	return true;
}

void Grid::connectBlocks() {
	
	BlockBase* block = NULL;
	
	for (s32 y = 0; y < GRID_HEIGHT; ++y) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {
			block = getBlockAt(x, y);
			
			if (block == NULL) continue;
			
			block->connect(getBlockAt(x, y - 1),
						   getBlockAt(x + 1, y),
						   getBlockAt(x, y + 1),
						   getBlockAt(x - 1, y));
		}
	}
}

bool Grid::animate() {

	bool result = false;

	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		
		if (_data[i] == NULL) continue;
		
		if (_data[i]->isExploded()) {
			delete _data[i];
			_data[i] = NULL;
			result = true;
		} else if (_data[i]->isExploding()) {
			result = true;
		} else if (_data[i]->isLanding()) {
			result = true;
		}

		if (_data[i] != NULL) _data[i]->animate();
	}

	for (s32 i = 0; i < GRID_WIDTH; ++i) {
		if (_columnOffsets[i] > 0) {
			--_columnOffsets[i];
			result = true;
		}
	}

	return result;
}

s32 Grid::getColumnHeight(s32 column) const {
	s32 height = 0;

	for (s32 y = Grid::GRID_HEIGHT - 1; y >= 0; --y) {
		if (getBlockAt(column, y) != NULL) {
			++height;
		} else {
			break;
		}
	}

	return height;
}

void Grid::addGarbage(s32 count) {
	s32 columnHeights[6];
	s32 columns[6];
	s32 items = 0;

	for (s32 i = 0; i < 6; ++i) {
		columnHeights[i] = -1;
	}

	// Add all column heights to the array in sorted order
	for (s32 i = 0; i < 6; ++i) {
		s32 height = getColumnHeight(i);
		s32 insertPoint = 0;

		// Locate where to insert this value
		for (s32 j = 0; j < items; ++j) {
			if (height < columnHeights[j]) {

				// Shuffle all items back one space to create a gap for the new
				// value
				for (s32 k = items; k > j; --k) {
					columnHeights[k] = columnHeights[k - 1];
					columns[k] = columns[k - 1];
				}
				break;
			}

			insertPoint++;
		}

		columnHeights[insertPoint] = height;
		columns[insertPoint] = i;
		++items;
	}

	// Add all garbage
	s32 activeColumns = 1;
	s32 y = columnHeights[0];

	if (count >= GRID_WIDTH) SoundPlayer::playGarbageBig(_playerNumber);

	while (count > 0) {

		s32 oldCount = count;

		while (activeColumns < 6 && columnHeights[activeColumns] <= y) ++activeColumns;

		for (s32 i = 0; i < activeColumns; ++i) {

			// Find a free block
			s32 garbageY = 0;
			while (getBlockAt(columns[i], garbageY) != NULL && garbageY < GRID_HEIGHT) {
				garbageY++;
			}

			// If we couldn't find a free block we'll try it in the next column
			// instead
			if (garbageY == GRID_HEIGHT) continue;

			setBlockAt(columns[i], garbageY, new GarbageBlock());

			--count;

			if (count == 0) break;
		}

		// If we failed to place the block the grid must be full
		if (oldCount == count) return;

		++y;
	}
}
