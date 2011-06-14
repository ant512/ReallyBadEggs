#include <nds.h>

#include <woopsiarray.h>

#include "hardware.h"
#include "pad.h"
#include "grid.h"

void liveTest() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	Grid grid;

	while (grid.dropBlocks()) {

		grid.render(0, 0, gfx);

		for (s32 i = 0; i < 10; ++i) {
			Hardware::waitForVBlank();
		}
	}

	while (1) {
		grid.addLiveBlocks();

		while (grid.hasLiveBlocks()) {

			const Pad& pad = Hardware::getPad();

			if (pad.isLeftNewPress() || pad.isLeftRepeat()) {
				grid.moveLiveBlocksLeft();
			} else if (pad.isRightNewPress() || pad.isRightRepeat()) {
				grid.moveLiveBlocksRight();
			} else if (pad.isDownNewPress() || pad.isDownRepeat()) {
				grid.dropLiveBlocks();
			} else if (pad.isANewPress()) {
				grid.rotateLiveBlocksClockwise();
			} else if (pad.isBNewPress()) {
				grid.rotateLiveBlocksAntiClockwise();
			}

			grid.render(0, 0, gfx);

			Hardware::waitForVBlank();
		}

		bool repeat = true;

		while (repeat) {

			while (grid.dropBlocks()) {

				grid.render(0, 0, gfx);

				for (s32 i = 0; i < 5; ++i) {
					Hardware::waitForVBlank();
				}
			}

			grid.connectBlocks();

			repeat = grid.removeChains();
		}
	}

	for (s32 i = 0; i < 10; ++i) {
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

	liveTest();

	Hardware::shutdown();
	return 0;
}
