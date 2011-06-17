#include <nds.h>

#include <graphics.h>
#include <pad.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "aicontroller.h"
#include "blockserver.h"
#include "gamefont.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "playercontroller.h"
#include "twoplayerbgbmp.h"

int main(int argc, char* argv[]) {
	Hardware::init();

	TwoPlayerBgBmp background;
	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
	gfx->drawBitmap(0, 0, background.getWidth(), background.getHeight(), &background, 0, 0);
	Hardware::getTopBuffer()->buffer();

	BlockServer* blockServer = new BlockServer(2, 4);

	s32 runnerX = Grid::BLOCK_SIZE;
	s32 aiRunnerX = SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE;
	s32 runnerWidth = Grid::GRID_WIDTH * Grid::BLOCK_SIZE;
	s32 runnerHeight = Grid::GRID_HEIGHT * Grid::BLOCK_SIZE;

	// Player 1
	Grid* grid = new Grid(3);
	PlayerController* controller = new PlayerController();
	GridRunner runner(controller, grid, blockServer, 0, runnerX);
	
	// Player 2
	Grid* aiGrid = new Grid(3);
	AIController* aiController = new AIController();
	GridRunner aiRunner(aiController, aiGrid, blockServer, 1, aiRunnerX);

	// We have to set the aiController's GridRunner after constructing the
	// runner because the runner needs a pointer to the controller and the
	// controller needs a pointer to the runner - argh
	aiController->setGridRunner(&aiRunner);

	while (1) {

		// Check for pause mode
		const Pad& pad = Hardware::getPad();

		if (pad.isStartNewPress()) {

			// Pause mode

			WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
			gfx->drawFilledRect(runnerX, 0, runnerWidth, runnerHeight, woopsiRGB(0, 0, 0));
			gfx->drawFilledRect(aiRunnerX, 0, runnerWidth, runnerHeight, woopsiRGB(0, 0, 0));

			WoopsiGfx::WoopsiString str("Paused");
			GameFont font;

			s32 textX = (runnerWidth - font.getStringWidth(str)) / 2;
			s32 textY = (runnerHeight - font.getHeight()) / 2;

			gfx->drawText(runnerX + textX, textY, &font, str, 0, str.getLength(), woopsiRGB(31, 31, 0));
			gfx->drawText(aiRunnerX + textX, textY, &font, str, 0, str.getLength(), woopsiRGB(31, 31, 0));

			// Need to wait for another vblank so that the newpress state of the
			// Start button is cleared
			Hardware::waitForVBlank();

			while (!pad.isStartNewPress()) {
				Hardware::waitForVBlank();
			}

		} else {

			// Standard mode
			runner.iterate();
			aiRunner.iterate();

			if (aiRunner.getOutgoingGreyBlockCount() > 0) {
				runner.addIncomingGreyBlocks(aiRunner.getOutgoingGreyBlockCount());
				aiRunner.clearOutgoingGreyBlockCount();
			}

			if (runner.getOutgoingGreyBlockCount() > 0) {
				aiRunner.addIncomingGreyBlocks(runner.getOutgoingGreyBlockCount());
				runner.clearOutgoingGreyBlockCount();
			}
		}

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
