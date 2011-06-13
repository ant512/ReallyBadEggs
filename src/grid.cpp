#include "grid.h"
#include "blockbase.h"

Grid::Grid() {
	_data = new BlockBase*[GRID_WIDTH * GRID_HEIGHT];
}

Grid::~Grid() {
	clear();

	delete[] _data;
}

BlockBase* Grid::getBlockAt(s32 x, s32 y) const {
	if (x < 0 || x >= GRID_WIDTH) return NULL;
	if (y < 0 || y >= GRID_HEIGHT) return NULL;

	return _data[x + (y * GRID_WIDTH)];
}

void Grid::setBlockAt(s32 x, s32 y, BlockBase* block) {
	BlockBase* currentBlock = getBlockAt(x, y);

	if (currentBlock == block) return;
	if (currentBlock != NULL) delete currentBlock;
	
	_data[x + (y * GRID_WIDTH)] = block;
}

void Grid::render(WoopsiGfx::Graphics* gfx) {
	
}

void Grid::iterate() {

}

void Grid::clear() {
	for (s32 i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		if (_data[i] != NULL) delete _data[i];
		_data[i] = NULL;
	}
}

bool Grid::removeConnectedBlocks() {

	// Build up an array that shows the number of connections for every block in
	// the grid.
	u8* connections = new u8[GRID_WIDTH * GRID_HEIGHT];

	for (s32 y = 0; y < GRID_HEIGHT; ++y) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {
			updateConnections(x, y, 0, connections); 
		}
	}

	bool removed = false;

	// Remove any blocks with a connection count higher than or equal to the max
	for (s32 y = 0; y < GRID_HEIGHT; ++y) {
		for (s32 x = 0; x < GRID_WIDTH; ++x) {
			if (connections[x + (y * GRID_HEIGHT)] >= MAX_CONNECTIONS) {
				_data[x + (y * GRID_HEIGHT)] = 0;
				removed = true;
			}
		}
	}

	delete[] connections;

	return removed;
}

s32 Grid::identifyConnections(s32 x, s32 y, s32 count, u8* data) {

	// Stop if we've already examined this block
	if (data[x + (y * GRID_WIDTH)] > 0) return count - 1;

	u8 block = getBlockAt(x, y);

	// Set the count data to 1 to prevent us examining this block again and
	// getting stuck in a cycle in the graph.  We overwrite the data later
	// with the correct value.
	data[x + (y * GRID_WIDTH)] = 1;

	// Stop if there is no block - we don't track connections with empty blocks
	if (block == 0) {
		data[x + (y * GRID_WIDTH)] = 0;
		return 0;

	// Recursively update the connection count of the block on the left
	if (getBlockAt(x - 1, y) == block) {
		count = identifyConnections(x - 1, y, count + 1, data);
	}

	// Recursively update the connection count of the block on the right
	if (getBlockAt(x + 1, y) == block) {
		count = identifyConnections(x + 1, y, count + 1, data);
	}

	// Recursively update the connection count of the block above
	if (getBlockAt(x, y - 1) == block) {
		count = identifyConnections(x, y - 1, count + 1, data);
	}
	
	// Recursively update the connection count of the block below
	if (getBlockAt(x, y + 1) == block) {
		count = identifyConnections(x, y + 1, count + 1, data);
	}

	data[x + (y * GRID_WIDTH)] = count;
	
	return count;
}
