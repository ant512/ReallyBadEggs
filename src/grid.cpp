#include "grid.h"

Grid::Grid() {
	_data = new u8[GRID_WIDTH * GRID_HEIGHT];
}

Grid::~Grid() {
	delete[] _data;
}

void Grid::render(WoopsiGfx::Graphics* gfx) {
	
}

void Grid::iterate() {

}
