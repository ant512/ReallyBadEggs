#include "grid.h"

Grid::Grid() {
	_data = new u8[GRID_WIDTH * GRID_HEIGHT];
	_liveBlocks = new Point[2];
	_hasLiveBlocks = false;

	clear();
}

Grid::~Grid() {
	delete[] _data;
	delete[] _liveBlocks;
}

void Grid::clear() {
	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		_data[i] = 0;
	}
}

u8 Grid::getBlockAt(s32 x, s32 y) const {
	if (!isValidCoordinate(x, y)) return 0;

	return _data[x + (y * GRID_WIDTH)];
}

void Grid::setBlockAt(s32 x, s32 y, u8 block) {
	if (!isValidCoordinate(x, y)) return;

	_data[x + (y * GRID_WIDTH)] = block;
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
			if (_data[x + (y * GRID_WIDTH)] == 0) continue;

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

		Point point = chain[index];
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
			}

			// Whether or not the block was relevant doesn't matter; we've
			// checked it and don't want to check it again
			checkedData[point.x - 1 + (point.y * GRID_WIDTH)] = true;
		}

		if (point.x + 1 < GRID_WIDTH && !checkedData[point.x + 1 + (point.y * GRID_WIDTH)]) {
			if (getBlockAt(point.x + 1, point.y) == block) {

				Point adjacentPoint;
				adjacentPoint.x = point.x + 1;
				adjacentPoint.y = point.y;

				chain.push_back(adjacentPoint);
			}

			checkedData[point.x + 1 + (point.y * GRID_WIDTH)] = true;
		}

		if (point.y - 1 >= 0 && !checkedData[point.x + ((point.y - 1) * GRID_WIDTH)]) {
			if (getBlockAt(point.x, point.y - 1) == block) {

				Point adjacentPoint;
				adjacentPoint.x = point.x;
				adjacentPoint.y = point.y - 1;

				chain.push_back(adjacentPoint);
			}

			checkedData[point.x + ((point.y - 1) * GRID_WIDTH)] = true;
		}

		if (point.y + 1 < GRID_HEIGHT && !checkedData[point.x + ((point.y + 1) * GRID_WIDTH)]) {
			if (getBlockAt(point.x, point.y + 1) == block) {

				Point adjacentPoint;
				adjacentPoint.x = point.x;
				adjacentPoint.y = point.y + 1;

				chain.push_back(adjacentPoint);
			}

			checkedData[point.x + ((point.y + 1) * GRID_WIDTH)] = true;
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

			// Check if the block has landed on another
			u8 blockBelow = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y + 1);

			if (blockBelow > 0) {
				_hasLiveBlocks = false;
			}
		}
	}

	if (_hasLiveBlocks) {

		// Blocks are still live - drop them block to the next position
		for (s32 i = 0; i < 2; ++i) {
			u8 block = getBlockAt(_liveBlocks[i].x, _liveBlocks[i].y);
			setBlockAt(_liveBlocks[i].x, _liveBlocks[i].y + 1, block);

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
			if (getBlockAt(x, y + 1) == 0) {
				setBlockAt(x, y + 1, getBlockAt(x, y));
				setBlockAt(x, y, 0);

				hasDropped = true;
			}
		}
	}

	return hasDropped;
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

void Grid::iterate() {

}
