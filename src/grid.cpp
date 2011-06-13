#include "grid.h"

Grid::Grid() {
	_data = new u8[GRID_WIDTH * GRID_HEIGHT];
	_dirtyBlocks = new bool[GRID_WIDTH * GRID_HEIGHT];
	_liveBlocks = new Point[2];
	_hasLiveBlocks = false;

	clear();
}

Grid::~Grid() {
	delete[] _data;
	delete[] _dirtyBlocks;
	delete[] _liveBlocks;
}

void Grid::clear() {
	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		_data[i] = BLOCK_NONE;
		_dirtyBlocks[i] = false;
	}
}

u8 Grid::getBlockAt(s32 x, s32 y) const {
	if (!isValidCoordinate(x, y)) return 0;

	return _data[x + (y * GRID_WIDTH)];
}

void Grid::setBlockAt(s32 x, s32 y, u8 block) {
	if (!isValidCoordinate(x, y)) return;

	_data[x + (y * GRID_WIDTH)] = block;
	_dirtyBlocks[x + (y * GRID_WIDTH)] = true;
}

bool Grid::isValidCoordinate(s32 x, s32 y) const {
	if (x < 0) return false;
	if (x >= GRID_WIDTH) return false;
	if (y < 0) return false;
	if (y >= GRID_HEIGHT) return false;

	return true;
}

s32 Grid::removeChains() {
	s32 score = 0;
	WoopsiArray<WoopsiArray<Point>*> chains;

	getChains(chains);

	for (s32 i = 0; i < chains.size(); ++i) {

		// TODO: Fix scoring to use correct values
		score += chains[i]->size() * 10;

		for (s32 j = 0; j < chains[i]->size(); ++j) {

			Point& point = chains[i]->at(j);

			setBlockAt(point.x, point.y, BLOCK_NONE);
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

			// Skip if block is blank
			if (getBlockAt(x, y) == BLOCK_NONE) continue;

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
		u8 block = getBlockAt(point.x, point.y);

		// Check if the block on the left of this is part of the chain.  Ignore
		// the block if it has already been checked.
		if (point.x - 1 >= 0 && !checkedData[point.x - 1 + (point.y * GRID_WIDTH)]) {

			if (getBlockAt(point.x - 1, point.y) == block) {

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

		if (point.x + 1 < GRID_WIDTH && !checkedData[point.x + 1 + (point.y * GRID_WIDTH)]) {
			if (getBlockAt(point.x + 1, point.y) == block) {

				Point adjacentPoint;
				adjacentPoint.x = point.x + 1;
				adjacentPoint.y = point.y;

				chain.push_back(adjacentPoint);

				checkedData[point.x + 1 + (point.y * GRID_WIDTH)] = true;
			}
		}

		if (point.y - 1 >= 0 && !checkedData[point.x + ((point.y - 1) * GRID_WIDTH)]) {
			if (getBlockAt(point.x, point.y - 1) == block) {

				Point adjacentPoint;
				adjacentPoint.x = point.x;
				adjacentPoint.y = point.y - 1;

				chain.push_back(adjacentPoint);

				checkedData[point.x + ((point.y - 1) * GRID_WIDTH)] = true;
			}
		}

		if (point.y + 1 < GRID_HEIGHT && !checkedData[point.x + ((point.y + 1) * GRID_WIDTH)]) {
			if (getBlockAt(point.x, point.y + 1) == block) {

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
		} else {

			// Skip the top block if arranged vertically - we don't care that
			// the top block is on top of the bottom block
			if (_liveBlocks[0].x == _liveBlocks[1].x && i == 0) continue;

			// Check if the block has landed on another
			u8 blockBelow = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y + 1);

			if (blockBelow != BLOCK_NONE) {
				_hasLiveBlocks = false;
			}
		}
	}

	if (_hasLiveBlocks) {

		// Blocks are still live - drop them to the next position.  Drop block
		// 1 first as when vertical 1 is always below
		for (s32 i = 1; i >= 0; --i) {
			u8 block = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
			setBlockAt(_liveBlocks[i].x, _liveBlocks[i].y + 1, block);
			setBlockAt(_liveBlocks[i].x, _liveBlocks[i].y, BLOCK_NONE);

			// Update the live block co-ordinates
			++_liveBlocks[i].y;
		}
	}
}

bool Grid::dropBlocks() {

	// Do not allow blocks to drop if we have live blocks
	if (_hasLiveBlocks) return false;

	bool hasDropped = false;

	// Drop starts at the second row from the bottom of the grid as there's no
	// point in dropping the bottom row
	for (s32 y = GRID_HEIGHT - 2; y >= 0; --y) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {

			// Ignore this block if it's empty
			if (getBlockAt(x, y) == 0) continue;

			// Drop the current block if the block below is empty
			if (getBlockAt(x, y + 1) == BLOCK_NONE) {
				setBlockAt(x, y + 1, getBlockAt(x, y));
				setBlockAt(x, y, BLOCK_NONE);

				hasDropped = true;
			}
		}
	}

	return hasDropped;
}

void Grid::renderDirty(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	s32 renderX = 0;
	s32 renderY = 0;

	for (s32 blockY = 0; blockY < GRID_HEIGHT; ++blockY) {
		for (s32 blockX = 0; blockX < GRID_WIDTH; ++blockX) {

			// Only redraw dirty blocks
			if (!_dirtyBlocks[blockX + (blockY * GRID_WIDTH)]) continue;

			renderX = x + (blockX * BLOCK_SIZE);
			renderY = y + (blockY * BLOCK_SIZE);

			switch (getBlockAt(blockX, blockY)) {
				case BLOCK_NONE:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 0, 0));
					break;
				case BLOCK_RED:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(31, 0, 0));
					break;
				case BLOCK_GREEN:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 31, 0));
					break;
				case BLOCK_BLUE:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 0, 31));
					break;
				case BLOCK_GREY:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(9, 9, 9));
					break;
			}
		}
	}
}

void Grid::render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	s32 renderX = 0;
	s32 renderY = 0;

	for (s32 blockY = 0; blockY < GRID_HEIGHT; ++blockY) {
		for (s32 blockX = 0; blockX < GRID_WIDTH; ++blockX) {

			renderX = x + (blockX * BLOCK_SIZE);
			renderY = y + (blockY * BLOCK_SIZE);

			switch (getBlockAt(blockX, blockY)) {
				case BLOCK_NONE:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 0, 0));
					break;
				case BLOCK_RED:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(31, 0, 0));
					break;
				case BLOCK_GREEN:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 31, 0));
					break;
				case BLOCK_BLUE:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(0, 0, 31));
					break;
				case BLOCK_GREY:
					gfx->drawFilledRect(renderX, renderY, BLOCK_SIZE, BLOCK_SIZE, woopsiRGB(9, 9, 9));
					break;
			}
		}
	}
}

void Grid::setLiveBlocks(u8 block1, u8 block2) {

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

bool Grid::hasLiveBlocks() const {
	return _hasLiveBlocks;
}

void Grid::moveLiveBlocksLeft() {
	if (!_hasLiveBlocks) return;

	bool canMove = true;

	// 0 block should always be on the left or at the top
	if (_liveBlocks[0].x == 0) canMove = false;
	if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != BLOCK_NONE) canMove = false;

	// Check 1 block if it is below the 0 block
	if (_liveBlocks[0].x == _liveBlocks[1].x) {
		if (getBlockAt(_liveBlocks[1].x - 1, _liveBlocks[1].y) != BLOCK_NONE) canMove = false;
	}

	if (canMove) {
		for (s32 i = 0; i < 2; ++i) {
			setBlockAt(_liveBlocks[i].x - 1, _liveBlocks[i].y, getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y));
			setBlockAt(_liveBlocks[i].x, _liveBlocks[i].y, BLOCK_NONE);
			--_liveBlocks[i].x;
		}
	}
}

void Grid::moveLiveBlocksRight() {
	if (!_hasLiveBlocks) return;

	bool canMove = true;

	// 1 block should always be on the right or at the bottom
	if (_liveBlocks[1].x == GRID_WIDTH - 1) canMove = false;
	if (getBlockAt(_liveBlocks[1].x + 1, _liveBlocks[1].y) != BLOCK_NONE) canMove = false;

	// Check 0 block if it is above the 1 block
	if (_liveBlocks[0].x == _liveBlocks[1].x) {
		if (getBlockAt(_liveBlocks[0].x + 1, _liveBlocks[0].y) != BLOCK_NONE) canMove = false;
	}

	if (canMove) {
		for (s32 i = 1; i >= 0; --i) {
			setBlockAt(_liveBlocks[i].x + 1, _liveBlocks[i].y, getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y));
			setBlockAt(_liveBlocks[i].x, _liveBlocks[i].y, BLOCK_NONE);
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
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1) != BLOCK_NONE) return;

		// Perform the rotation

		// Move the right block down one place
		setBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1, getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y));
		++_liveBlocks[1].y;

		// Move the left block right one place
		setBlockAt(_liveBlocks[0].x + 1, _liveBlocks[0].y, getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y));
		setBlockAt(_liveBlocks[0].x, _liveBlocks[0].y, BLOCK_NONE);
		++_liveBlocks[0].x;

	} else {

		// Swapping to horizontal

		// Cannot swap if the blocks are at the left edge of the well
		if (_liveBlocks[0].x == 0) return;

		// Cannot swap if the block to the left of the block at the top is populated
		if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != BLOCK_NONE) return;

		// Perform the rotation

		// Move the bottom block up and left
		setBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y, getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y));
		setBlockAt(_liveBlocks[1].x, _liveBlocks[1].y, BLOCK_NONE);

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
		if (getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1) != BLOCK_NONE) return;

		// Perform the rotation

		// Move the left block down and right
		setBlockAt(_liveBlocks[1].x, _liveBlocks[1].y + 1, getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y));
		setBlockAt(_liveBlocks[0].x, _liveBlocks[0].y, BLOCK_NONE);

		// 0 block should always be at the top
		_liveBlocks[0].x = _liveBlocks[1].x;
		_liveBlocks[0].y = _liveBlocks[1].y;
		++_liveBlocks[1].y;

	} else {

		// Swapping to horizontal

		// Cannot swap if the blocks are at the left edge of the well
		if (_liveBlocks[0].x == 0) return;

		// Cannot swap if the block to the left of the block at the top is populated
		if (getBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y) != BLOCK_NONE) return;

		// Perform the rotation

		// Move the top block left
		setBlockAt(_liveBlocks[0].x - 1, _liveBlocks[0].y, getBlockAt(_liveBlocks[0].x, _liveBlocks[0].y));
		--_liveBlocks[0].x;

		// Move the bottom block up
		setBlockAt(_liveBlocks[1].x, _liveBlocks[1].y - 1, getBlockAt(_liveBlocks[1].x, _liveBlocks[1].y));
		setBlockAt(_liveBlocks[1].x, _liveBlocks[1].y, BLOCK_NONE);
		--_liveBlocks[1].y;
	}
}

void Grid::addLiveBlocks() {
	u8 block1 = 1 + (rand() % 3);
	u8 block2 = 1 + (rand() % 3);

	setLiveBlocks(block1, block2);
}

void Grid::iterate() {

}
