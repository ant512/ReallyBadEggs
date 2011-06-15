#include <nds.h>

#include <woopsiarray.h>

#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "pad.h"
#include "playercontroller.h"

int main(int argc, char* argv[]) {
	Hardware::init();

	PlayerController controller;
	GridRunner runner(&controller, 4, 3);

	while (1) {
		runner.iterate(0, 0, Hardware::getTopGfx());
		Hardware::waitForVBlank();
	}


	Hardware::shutdown();
	return 0;
}
