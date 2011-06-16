#include <nds.h>

#include <woopsiarray.h>

#include "aicontroller.h"
#include "blockserver.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "pad.h"
#include "playercontroller.h"

int main(int argc, char* argv[]) {
	Hardware::init();

	BlockServer* blockServer = new BlockServer(2, 4);

	// Player 1
	Grid* grid = new Grid(3);
	ControllerBase* controller = new PlayerController();
	GridRunner runner(controller, grid, blockServer, 0);
	
	// Player 2
	Grid* aiGrid = new Grid(3);
	ControllerBase* aiController = new AIController(aiGrid);
	GridRunner aiRunner(aiController, aiGrid, blockServer, 1);
	
	s32 runnerX = Grid::BLOCK_SIZE;
	s32 aiRunnerX = SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE;

	while (1) {
		runner.iterate(runnerX, 0, Hardware::getTopGfx());
		aiRunner.iterate(aiRunnerX, 0, Hardware::getTopGfx());
		Hardware::waitForVBlank();
	}

	delete blockServer;

	delete grid;
	delete controller;

	delete aiGrid;
	delete aiController;

	Hardware::shutdown();
	return 0;
}
