#include "grid.h"
#include "greyblock.h"
#include "redblock.h"
#include "purpleblock.h"
#include "yellowblock.h"
#include "blueblock.h"

Grid::Grid(s32 startingHeight) {
	_data = new BlockBase*[GRID_WIDTH * GRID_HEIGHT];
	_liveBlocks = new Point[2];
	_hasLiveBlocks = false;

	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		_data[i] = NULL;
	}

	// Add rows of greys
	if (startingHeight > GRID_HEIGHT) startingHeight = GRID_HEIGHT;

	for (s32 row = 0; row < startingHeight; ++row) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {
			s32 y = GRID_HEIGHT - 1 - row;

			setBlockAt(x, y, new GreyBlock());
		}
	}
}

Grid::~Grid() {
	clear();
	delete[] _data;
	delete[] _liveBlocks;
}

void Grid::clear() {

	// Delete everything in the grid
	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		if (_data[i] != NULL) {
			delete _data[i];
			_data[i] = NULL;
		}
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

s32 Grid::explodeChains() {
	s32 score = 0;
	WoopsiArray<WoopsiArray<Point>*> chains;

	getChains(chains);

	for (s32 i = 0; i < chains.size(); ++i) {

		// TODO: Fix scoring to use correct values and include greys
		score += chains[i]->size() * 10;

		for (s32 j = 0; j < chains[i]->size(); ++j) {

			Point& point = chains[i]->at(j);

			getBlockAt(point.x, point.y)->explode();
			//setBlockAt(point.x, point.y, NULL);

			// Remove any adjacent greys

			// RTII - yuck, but fastest way for me to code it
			GreyBlock* grey = dynamic_cast<GreyBlock*>(getBlockAt(point.x - 1, point.y));
			if (grey != NULL) {
				getBlockAt(point.x - 1, point.y)->explode();
				//setBlockAt(point.x - 1, point.y, NULL);
			}

			grey = dynamic_cast<GreyBlock*>(getBlockAt(point.x + 1, point.y));
			if (grey != NULL) {
				getBlockAt(point.x + 1, point.y)->explode();
				//setBlockAt(point.x + 1, point.y, NULL);
			}

			grey = dynamic_cast<GreyBlock*>(getBlockAt(point.x, point.y - 1));
			if (grey != NULL) {
				getBlockAt(point.x, point.y - 1)->explode();
				//setBlockAt(point.x, point.y - 1, NULL);
			}

			grey = dynamic_cast<GreyBlock*>(getBlockAt(point.x, point.y + 1));
			if (grey != NULL) {
				getBlockAt(point.x, point.y + 1)->explode();
				//setBlockAt(point.x, point.y + 1, NULL);
			}
		}

		delete chains[i];
	}

	return score;
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

	// Check both live blocks for collisions before we try to drop them.  This
	// prevents us from getting into a situation in which one of the pair drops
	// and the other hits something
	for (s32 i = 0; i < 2; ++i) {

		// Check if block is at bottom of grid
		if (_liveBlocks[i].y + 1 >= GRID_HEIGHT) {
			_hasLiveBlocks = false;

			BlockBase* block = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
			block->land();
		} else {

			// Check if the block has landed on another
			BlockBase* blockBelow = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y + 1);

			if (blockBelow != NULL) {

				// Do not land if the block below is also falling
				if (!blockBelow->isFalling()) {
					_hasLiveBlocks = false;

					BlockBase* block = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
					block->land();
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
}

bool Grid::dropBlocks() {

	// Do not allow blocks to drop if we have live blocks
	if (_hasLiveBlocks) return false;

	bool hasDropped = false;

	// Everything on the bottom row should have landed
	for (s32 x = 0; x < GRID_WIDTH; ++x) {
		BlockBase* block = getBlockAt(x, GRID_HEIGHT - 1);

		if (block->isFalling()) {
			block->land();
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
				block->land();
			}
		}
	}

	return hasDropped;
}

void Grid::render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	s32 renderX = 0;
	s32 renderY = 0;

	for (s32 blockX = 0; blockX < GRID_WIDTH; ++blockX) {
		for (s32 blockY = GRID_HEIGHT - 1; blockY >= 0; --blockY) {
		
			renderX = x + (blockX * BLOCK_SIZE);
			renderY = y + (blockY * BLOCK_SIZE);

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
	if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != NULL) canMove = false;

	// Check 1 block if it is below the 0 block
	if (_liveBlocks[0].x == _liveBlocks[1].x) {
		if (getBlockAt(_liveBlocks[1].x - 1, _liveBlocks[1].y) != NULL) canMove = false;
	}

	if (canMove) {
		for (s32 i = 0; i < 2; ++i) {
			moveBlock(_liveBlocks[i].x, _liveBlocks[i].y, _liveBlocks[i].x - 1, _liveBlocks[i].y);
			--_liveBlocks[i].x;
		}
	}
}

void Grid::moveLiveBlocksRight() {
	if (!_hasLiveBlocks) return;

	bool canMove = true;

	// 1 block should always be on the right or at the bottom
	if (_liveBlocks[1].x == GRID_WIDTH - 1) canMove = false;
	if (getBlockAt(_liveBlocks[1].x + 1, _liveBlocks[1].y) != NULL) canMove = false;

	// Check 0 block if it is above the 1 block
	if (_liveBlocks[0].x == _liveBlocks[1].x) {
		if (getBlockAt(_liveBlocks[0].x + 1, _liveBlocks[0].y) != NULL) canMove = false;
	}

	if (canMove) {
		for (s32 i = 1; i >= 0; --i) {
			moveBlock(_liveBlocks[i].x, _liveBlocks[i].y, _liveBlocks[i].x + 1, _liveBlocks[i].y);
			++_liveBlocks[i].x;
		}
	}
}

void Grid::rotateLiveBlocksClockwise() {
	if (!_hasLiveBlocks) return;

	// Determine whether the blocks swap to a vertical or horizontal arrangement
	if (_liveBlocks[0].y == _liveBlocks[1].y) {

		// Swapping to vertical

		// Cannot swap if the blocks are at the bottom of the well
		if (_liveBlocks[0].y == GRID_HEIGHT - 1) return;

		// Cannot swap if the block below the block on the right is populated
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1) != NULL) return;

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

		// Perform the rotation

		// Move the bottom block up and left
		moveBlock(_liveBlocks[1].x, _liveBlocks[1].y, _liveBlocks[0].x - 1, _liveBlocks[0].y);

		// 0 block should always be on the left
		_liveBlocks[1].x = _liveBlocks[0].x;
		_liveBlocks[1].y = _liveBlocks[0].y;

		--_liveBlocks[0].x;
	}
}

void Grid::rotateLiveBlocksAntiClockwise() {
	if (!_hasLiveBlocks) return;

	// Determine whether the blocks swap to a vertical or horizontal arrangement
	if (_liveBlocks[0].y == _liveBlocks[1].y) {

		// Swapping to vertical

		// Cannot swap if the blocks are at the bottom of the well
		if (_liveBlocks[0].y == GRID_HEIGHT - 1) return;

		// Cannot swap if the block below the block on the right is populated
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1) != NULL) return;

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

		// Perform the rotation

		// Move the top block left
		moveBlock(_liveBlocks[0].x, _liveBlocks[0].y, _liveBlocks[0].x - 1, _liveBlocks[0].y);
		--_liveBlocks[0].x;

		// Move the bottom block up
		moveBlock(_liveBlocks[1].x, _liveBlocks[1].y, _liveBlocks[1].x, _liveBlocks[1].y - 1);
		--_liveBlocks[1].y;
	}
}

void Grid::addLiveBlocks(BlockBase* block1, BlockBase* block2) {

	// Do not add more live blocks if we have blocks already
	if (_hasLiveBlocks) return;

	// Live blocks always appear at the same co-ordinates
	setBlockAt(2, 0, block1);
	setBlockAt(3, 0, block2);

	_liveBlocks[0].x = 2;
	_liveBlocks[0].y = 0;

	_liveBlocks[1].x = 3;
	_liveBlocks[1].y = 0;

	_hasLiveBlocks = true;
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

	return result;
}
