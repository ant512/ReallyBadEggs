

#include "grid.h"

Grid::Grid() {
	_data = new u8[GRID_WIDTH * GRID_HEIGHT];
	clear();
}

Grid::~Grid() {
	delete[] _data;
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

void Grid::getChains(WoopsiArray<WoopsiArray<Point>*>& chains) {

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

void Grid::getChain(s32 x, s32 y, WoopsiArray<Point>& chain, bool* checkedData) {

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

void Grid::render(WoopsiGfx::Graphics* gfx) {
	
}

void Grid::iterate() {

}
