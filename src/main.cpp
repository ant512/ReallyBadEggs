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
	GridRunner runner(&controller, 4);

	while (1) {
		runner.iterate();
		Hardware::waitForVBlank();
	}


	Hardware::shutdown();
	return 0;
}
