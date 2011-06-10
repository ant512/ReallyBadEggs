#include <nds.h>

#include "hardware.h"

int main(int argc, char* argv[]) {
	Hardware::init();

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

	Hardware::shutdown();

	return 0;
}
