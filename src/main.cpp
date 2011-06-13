#include <nds.h>

#include <woopsiarray.h>

#include "hardware.h"
#include "grid.h"

void dropTest() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	Grid grid;

	grid.setBlockAt(0, 0, 1);
	grid.setBlockAt(1, 0, 1);
	grid.setBlockAt(2, 0, 1);
	grid.setBlockAt(1, 1, 1);
	grid.setBlockAt(1, 2, 1);
	grid.setBlockAt(0, 2, 1);
	grid.setBlockAt(0, 3, 1);

	grid.setBlockAt(2, 2, 2);
	grid.setBlockAt(2, 3, 2);
	grid.setBlockAt(2, 1, 2);

	grid.render(0, 0, gfx);
	grid.removeChains();

	for (s32 i = 0; i < 10; ++i) {
		Hardware::waitForVBlank();
	}

	while (grid.dropBlocks()) {

		grid.render(0, 0, gfx);

		for (s32 i = 0; i < 10; ++i) {
			Hardware::waitForVBlank();
		}
	}
}

void chainTest() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	Grid grid;

	grid.setBlockAt(0, 0, 1);
	grid.setBlockAt(1, 0, 1);
	grid.setBlockAt(2, 0, 1);
	grid.setBlockAt(1, 1, 1);
	grid.setBlockAt(1, 2, 1);
	grid.setBlockAt(0, 2, 1);
	grid.setBlockAt(0, 3, 1);

	grid.setBlockAt(2, 2, 2);
	grid.setBlockAt(2, 3, 2);
	grid.setBlockAt(2, 1, 2);

	WoopsiArray<Point> chain;

	bool* checkedData = new bool[Grid::GRID_WIDTH * Grid::GRID_HEIGHT];

	grid.getChain(1, 1, chain, checkedData);

	delete[] checkedData;

	s32 blockSize = 2;

	grid.render(0, 0, gfx);

	for (s32 i = 0; i < 10; ++i) {
		Hardware::waitForVBlank();
	}

	for (s32 i = 0; i < chain.size(); ++i) {
		gfx->drawFilledRect(chain[i].x * blockSize, chain[i].y * blockSize, blockSize, blockSize, woopsiRGB(31, 31, 31));
	}

	for (s32 i = 0; i < 10; ++i) {
		Hardware::waitForVBlank();
	}

	grid.removeChains();
	grid.render(0, 0, gfx);

	while (1) {
		Hardware::waitForVBlank();
	}
}

void padTest() {
	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	while (1) {
		const Pad& pad = Hardware::getPad();

		if (pad.isLeftHeld()) {
			gfx->drawFilledRect(10, 10, 10, 10, woopsiRGB(31, 0, 0));
		} else {
			gfx->drawFilledRect(10, 10, 10, 10, woopsiRGB(0, 0, 0));
		}

		Hardware::waitForVBlank();
	}
}

int main(int argc, char* argv[]) {
	Hardware::init();

	dropTest();

	Hardware::shutdown();
	return 0;
}
