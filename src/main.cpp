#include <nds.h>

#include <graphics.h>
#include <pad.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "smartaicontroller.h"
#include "blockserver.h"
#include "gamefont.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "playercontroller.h"
#include "twoplayerbgbmp.h"

#include "menuset.h"

enum GameState {
	GAME_STATE_ACTIVE = 0,
	GAME_STATE_PAUSED = 1,
	GAME_STATE_GAME_OVER = 2
};

void showText(s32 x, s32 y, s32 width, s32 height, const WoopsiGfx::WoopsiString& text) {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
	gfx->drawFilledRect(x, y, width, height, woopsiRGB(0, 0, 0));

	GameFont font;

	s32 textX = (width - font.getStringWidth(text)) / 2;
	s32 textY = (height - font.getHeight()) / 2;

	gfx->drawText(x + textX, textY, &font, text, 0, text.getLength(), woopsiRGB(31, 31, 0));

	Hardware::getTopBuffer()->buffer();
}

void showPauseScreen(s32 x1, s32 x2, s32 y, s32 width, s32 height) {
	showText(x1, y, width, height, "Paused");
	showText(x2, y, width, height, "Paused");
}

void showMenu() {
	GameFont font;
	MenuSet gameTypeMenu(10, 100, 100, 4, 1, 0, "Game Type");
	gameTypeMenu.addOption("Game A", 0);
	gameTypeMenu.addOption("Game B", 1);
	gameTypeMenu.addOption("Easy AI", 2);
	gameTypeMenu.addOption("Hard AI", 3);

	MenuSet startLevelMenu(10, 100, 100, 5, 2, 1, "Start Level");
	gameTypeMenu.addOption("0", 0);
	gameTypeMenu.addOption("1", 1);
	gameTypeMenu.addOption("2", 2);
	gameTypeMenu.addOption("3", 3);
	gameTypeMenu.addOption("4", 4);
	gameTypeMenu.addOption("5", 5);
	gameTypeMenu.addOption("6", 6);
	gameTypeMenu.addOption("7", 7);
	gameTypeMenu.addOption("8", 8);
	gameTypeMenu.addOption("9", 9);

	MenuSet startHeightMenu(10, 100, 100, 3, 2, 2, "Height");
	gameTypeMenu.addOption("0", 0);
	gameTypeMenu.addOption("1", 1);
	gameTypeMenu.addOption("2", 2);
	gameTypeMenu.addOption("3", 3);
	gameTypeMenu.addOption("4", 4);
	gameTypeMenu.addOption("5", 5);

	MenuSet coloursMenu(10, 100, 100, 3, 1, 3, "Block Colours");
	coloursMenu.addOption("4", 4);
	coloursMenu.addOption("5", 5);
	coloursMenu.addOption("6", 6);

	while (1) {
		gameTypeMenu.iterate();
		gameTypeMenu.render(&font, Hardware::getTopGfx());
		Hardware::waitForVBlank();
	}
}

int main(int argc, char* argv[]) {

	Hardware::init();

	showMenu();





	s32 runnerX = Grid::BLOCK_SIZE;
	s32 aiRunnerX = SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE;
	s32 runnerWidth = Grid::GRID_WIDTH * Grid::BLOCK_SIZE;
	s32 runnerHeight = Grid::GRID_HEIGHT * Grid::BLOCK_SIZE;
	s32 startLevel = 0;
	s32 startHeight = 0;
	s32 smartAI = true;
	GridRunner::GameType gameType = GridRunner::GAME_TYPE_TWO_PLAYER;
	GameState state = GAME_STATE_ACTIVE;

	// Set up background graphic
	TwoPlayerBgBmp background;
	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
	gfx->drawBitmap(0, 0, background.getWidth(), background.getHeight(), &background, 0, 0);
	Hardware::getTopBuffer()->buffer();

	BlockServer* blockServer = new BlockServer(2, 4);

	// Player 1
	Grid* grid = new Grid(startHeight);
	PlayerController* controller = new PlayerController();
	GridRunner runner(controller, grid, blockServer, 0, runnerX, gameType, startLevel);
	
	// Player 2
	Grid* aiGrid = new Grid(startHeight);
	SmartAIController* aiController = new SmartAIController(smartAI);
	GridRunner aiRunner(aiController, aiGrid, blockServer, 1, aiRunnerX, gameType, startLevel);

	// We have to set the aiController's GridRunner after constructing the
	// runner because the runner needs a pointer to the controller and the
	// controller needs a pointer to the runner - argh
	aiController->setGridRunner(&aiRunner);

	const Pad& pad = Hardware::getPad();

	while (1) {
		switch (state) {
			case GAME_STATE_ACTIVE:

				// Standard mode
				runner.iterate();
				aiRunner.iterate();

				if (runner.isDead() && !aiRunner.isDead()) {
					showText(runnerX, 0, runnerWidth, runnerHeight, "Loser");
					showText(aiRunnerX, 0, runnerWidth, runnerHeight, "Winner");
					state = GAME_STATE_GAME_OVER;
				} else if (aiRunner.isDead() && !runner.isDead()) {
					showText(runnerX, 0, runnerWidth, runnerHeight, "Winner");
					showText(aiRunnerX, 0, runnerWidth, runnerHeight, "Loser");
					state = GAME_STATE_GAME_OVER;
				} else if (aiRunner.isDead() && runner.isDead()) {
					showText(runnerX, 0, runnerWidth, runnerHeight, "Draw");
					showText(aiRunnerX, 0, runnerWidth, runnerHeight, "Draw");
					state = GAME_STATE_GAME_OVER;
				}

				if (runner.addIncomingGarbage(aiRunner.getOutgoingGarbageCount())) {
					aiRunner.clearOutgoingGarbageCount();
				}

				if (aiRunner.addIncomingGarbage(runner.getOutgoingGarbageCount())) {
					runner.clearOutgoingGarbageCount();
				}

				if (pad.isStartNewPress()) {
					showPauseScreen(runnerX, aiRunnerX, 0, runnerWidth, runnerHeight);
					state = GAME_STATE_PAUSED;
				}

				break;
			
			case GAME_STATE_PAUSED:
				if (pad.isStartNewPress()) {
					state = GAME_STATE_ACTIVE;
				}
				break;

			case GAME_STATE_GAME_OVER:
				// TODO: Check for start/A/B button and return to options screen
				break;
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
