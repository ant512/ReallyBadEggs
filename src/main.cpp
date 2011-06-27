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

#include "menu.h"

enum GameState {
	GAME_STATE_TITLE = 0,
	GAME_STATE_MENU = 1,
	GAME_STATE_ACTIVE = 2,
	GAME_STATE_PAUSED = 3,
	GAME_STATE_GAME_OVER = 4
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

	Menu menu;

	while (1) {
		menu.iterate();
		Hardware::waitForVBlank();
	}
}

int main(int argc, char* argv[]) {

	Hardware::init();

	s32 runnerX = Grid::BLOCK_SIZE;
	s32 aiRunnerX = SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE;
	s32 runnerWidth = Grid::GRID_WIDTH * Grid::BLOCK_SIZE;
	s32 runnerHeight = Grid::GRID_HEIGHT * Grid::BLOCK_SIZE;
	GridRunner::GameType gameType = GridRunner::GAME_TYPE_TWO_PLAYER;
	GameState state = GAME_STATE_TITLE;

	Menu* menu = new Menu();

	// Set up background graphic
	TwoPlayerBgBmp background;

	BlockServer* blockServer = NULL;

	// Player 1
	Grid* grid = NULL;
	PlayerController* controller = new PlayerController();
	GridRunner* runner = NULL;
	
	// Player 2
	Grid* aiGrid = NULL;
	SmartAIController* aiController = NULL;
	GridRunner* aiRunner = NULL;

	const Pad& pad = Hardware::getPad();

	s32 blanks = 0;

	while (1) {

		++blanks;

		switch (state) {
			case GAME_STATE_TITLE:

				showText(0, 0, 256, 192, "Really Bad Eggs");

				if (pad.isANewPress() || pad.isStartNewPress()) {
					
					WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
					gfx->drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, woopsiRGB(0, 0, 0));
					Hardware::getTopBuffer()->buffer();

					state = GAME_STATE_MENU;
				}
				break;

			case GAME_STATE_MENU:
				menu->iterate();

				if (!menu->isRunning()) {

					srand(blanks);

					switch (menu->getGameType()) {
						case 0:
							gameType = GridRunner::GAME_TYPE_A;
							blockServer = new BlockServer(1, menu->getColours());
							break;
						case 1:
							gameType = GridRunner::GAME_TYPE_B;
							blockServer = new BlockServer(1, menu->getColours());
							break;
						case 2:
							gameType = GridRunner::GAME_TYPE_TWO_PLAYER;
							
							blockServer = new BlockServer(2, menu->getColours());
							aiGrid = new Grid(menu->getStartHeight());
							aiController = new SmartAIController(false);
							aiRunner = new GridRunner(aiController, aiGrid, blockServer, 1, aiRunnerX, gameType, menu->getStartLevel());

							aiController->setGridRunner(aiRunner);
							break;
						case 3:
							gameType = GridRunner::GAME_TYPE_TWO_PLAYER;

							blockServer = new BlockServer(2, menu->getColours());
							aiGrid = new Grid(menu->getStartHeight());
							aiController = new SmartAIController(true);
							aiRunner = new GridRunner(aiController, aiGrid, blockServer, 1, aiRunnerX, gameType, menu->getStartLevel());

							aiController->setGridRunner(aiRunner);
							break;
					}

					grid = new Grid(menu->getStartHeight());
					runner = new GridRunner(controller, grid, blockServer, 0, runnerX, gameType, menu->getStartLevel());

					WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
					gfx->drawBitmap(0, 0, background.getWidth(), background.getHeight(), &background, 0, 0);
					Hardware::getTopBuffer()->buffer();

					state = GAME_STATE_ACTIVE;
				}

				break;

			case GAME_STATE_ACTIVE:

				// Standard mode
				runner->iterate();

				if (aiRunner == NULL) {
					if (runner->isDead()) {
						showText(runnerX, 0, runnerWidth, runnerHeight, "Game Over");
						state = GAME_STATE_GAME_OVER;
					}
				} else {
					aiRunner->iterate();

					if (runner->isDead() && !aiRunner->isDead()) {
						showText(runnerX, 0, runnerWidth, runnerHeight, "Loser");
						showText(aiRunnerX, 0, runnerWidth, runnerHeight, "Winner");
						state = GAME_STATE_GAME_OVER;
					} else if (aiRunner->isDead() && !runner->isDead()) {
						showText(runnerX, 0, runnerWidth, runnerHeight, "Winner");
						showText(aiRunnerX, 0, runnerWidth, runnerHeight, "Loser");
						state = GAME_STATE_GAME_OVER;
					} else if (aiRunner->isDead() && runner->isDead()) {
						showText(runnerX, 0, runnerWidth, runnerHeight, "Draw");
						showText(aiRunnerX, 0, runnerWidth, runnerHeight, "Draw");
						state = GAME_STATE_GAME_OVER;
					}

					if (runner->addIncomingGarbage(aiRunner->getOutgoingGarbageCount())) {
						aiRunner->clearOutgoingGarbageCount();
					}

					if (aiRunner->addIncomingGarbage(runner->getOutgoingGarbageCount())) {
						runner->clearOutgoingGarbageCount();
					}
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
				if (pad.isStartNewPress() || pad.isANewPress() || pad.isBNewPress()) {

					delete blockServer;

					delete runner;
					delete grid;

					delete aiRunner;
					delete aiGrid;
					delete aiController;

					blockServer = NULL;
					runner = NULL;
					grid = NULL;
					aiRunner = NULL;
					aiGrid = NULL;
					aiController = NULL;

					menu->setActiveMenu(1);

					state = GAME_STATE_MENU;
				}
				break;
		}

		Hardware::waitForVBlank();
	}

	delete menu;
	delete controller;

	Hardware::shutdown();
	return 0;
}
