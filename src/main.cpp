#include <nds.h>

#include <woopsiarray.h>

#include "hardware.h"
#include "grid.h"

void dropTest() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
	s32 blockSize = 5;

	Grid grid;

	grid.setBlockAt(0, 0, 2);
	grid.setBlockAt(1, 0, 2);
	grid.setBlockAt(2, 0, 2);
	grid.setBlockAt(1, 1, 2);
	grid.setBlockAt(1, 2, 2);
	grid.setBlockAt(0, 2, 2);
	grid.setBlockAt(0, 3, 2);

	while (1) {

		for (s32 y = 0; y < Grid::GRID_HEIGHT; ++y) {
			for (s32 x = 0; x < Grid::GRID_WIDTH; ++x) {
				if (grid.getBlockAt(x, y) == 2) {
					gfx->drawFilledRect(x * blockSize, y * blockSize, blockSize, blockSize, woopsiRGB(31, 0, 0));
				} else {
					gfx->drawFilledRect(x * blockSize, y * blockSize, blockSize, blockSize, woopsiRGB(5, 0, 0));
				}
			}
		}

		grid.dropBlocks();

		for (s32 i = 0; i < 10; ++i) {
			Hardware::waitForVBlank();
		}

		gfx->drawFilledRect(0, 0, 256, 192, 0);
	}
}

void chainTest() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	Grid grid;

	grid.setBlockAt(0, 0, 2);
	grid.setBlockAt(1, 0, 2);
	grid.setBlockAt(2, 0, 2);
	grid.setBlockAt(1, 1, 2);
	grid.setBlockAt(1, 2, 2);
	grid.setBlockAt(0, 2, 2);
	grid.setBlockAt(0, 3, 2);

	WoopsiArray<Point> chain;

	bool* checkedData = new bool[Grid::GRID_WIDTH * Grid::GRID_HEIGHT];

	grid.getChain(1, 1, chain, checkedData);

	delete[] checkedData;

	s32 blockSize = 5;

	for (s32 y = 0; y < Grid::GRID_HEIGHT; ++y) {
		for (s32 x = 0; x < Grid::GRID_WIDTH; ++x) {
			if (grid.getBlockAt(x, y) == 2) {
				gfx->drawFilledRect(x * blockSize, y * blockSize, blockSize, blockSize, woopsiRGB(31, 0, 0));
			} else {
				gfx->drawFilledRect(x * blockSize, y * blockSize, blockSize, blockSize, woopsiRGB(5, 0, 0));
			}
		}
	}

	for (s32 i = 0; i < chain.size(); ++i) {
		gfx->drawFilledRect(chain[i].x * blockSize, chain[i].y * blockSize, blockSize, blockSize, woopsiRGB(31, 31, 31));
	}

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
