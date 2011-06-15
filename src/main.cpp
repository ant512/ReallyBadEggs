#include <nds.h>

#include <woopsiarray.h>

#include "blockserver.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "pad.h"
#include "playercontroller.h"

int main(int argc, char* argv[]) {
	Hardware::init();

	BlockServer* blockServer = new BlockServer(1, 4);
	PlayerController* controller = new PlayerController();
	GridRunner runner(controller, 3, blockServer, 0);

	while (1) {
		runner.iterate(0, 0, Hardware::getTopGfx());
		Hardware::waitForVBlank();
	}

	delete blockServer;
	delete controller;

	Hardware::shutdown();
	return 0;
}
