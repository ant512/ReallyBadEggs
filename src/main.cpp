#include <nds.h>

#include <graphics.h>
#include <woopsiarray.h>

#include "aicontroller.h"
#include "blockserver.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "pad.h"
#include "playercontroller.h"
#include "twoplayerbgbmp.h"

int main(int argc, char* argv[]) {
	Hardware::init();

	TwoPlayerBgBmp background;
	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
	gfx->drawBitmap(0, 0, background.getWidth(), background.getHeight(), &background, 0, 0);
	Hardware::getTopBuffer()->buffer();

	BlockServer* blockServer = new BlockServer(2, 4);

	// Player 1
	Grid* grid = new Grid(3);
	ControllerBase* controller = new PlayerController();
	GridRunner runner(controller, grid, blockServer, 0, Grid::BLOCK_SIZE);
	
	// Player 2
	Grid* aiGrid = new Grid(3);
	ControllerBase* aiController = new AIController(aiGrid);
	GridRunner aiRunner(aiController, aiGrid, blockServer, 1, SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE);

	while (1) {
		runner.iterate();
		aiRunner.iterate();
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
