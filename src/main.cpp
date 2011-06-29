#include <nds.h>

#include <graphics.h>
#include <pad.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "blockserver.h"
#include "gamefont.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "menu.h"
#include "pausedbmp.h"
#include "playercontroller.h"
#include "simianzombielogobmp.h"
#include "smartaicontroller.h"
#include "soundplayer.h"
#include "twoplayerbgbmp.h"
#include "winnerbmp.h"

enum GameState {
	GAME_STATE_TITLE = 0,
	GAME_STATE_MENU = 1,
	GAME_STATE_ACTIVE = 2,
	GAME_STATE_PAUSED = 3,
	GAME_STATE_GAME_OVER = 4
};

void showText(s32 x, s32 y, s32 width, s32 height, const WoopsiGfx::WoopsiString& text) {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	GameFont font;

	s32 textX = (width - font.getStringWidth(text)) / 2;
	s32 textY = (height - font.getHeight()) / 2;

	gfx->drawText(x + textX, textY, &font, text, 0, text.getLength(), woopsiRGB(31, 31, 0));

	Hardware::getTopBuffer()->buffer();
}

void clearScreens() {
	Hardware::getTopGfx()->drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, woopsiRGB(0, 0, 0));
	Hardware::getBottomGfx()->drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, woopsiRGB(0, 0, 0));
	Hardware::getTopBuffer()->buffer();
}

int main(int argc, char* argv[]) {

	Hardware::init();
	SoundPlayer::init();

	s32 runnerX = Grid::BLOCK_SIZE;
	s32 aiRunnerX = SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE;
	s32 runnerWidth = Grid::GRID_WIDTH * Grid::BLOCK_SIZE;
	s32 runnerHeight = Grid::GRID_HEIGHT * Grid::BLOCK_SIZE;
	GridRunner::GameType gameType = GridRunner::GAME_TYPE_TWO_PLAYER;
	GameState state = GAME_STATE_TITLE;

	Menu* menu = new Menu();

	TwoPlayerBgBmp background;
	SimianZombieLogoBmp simianZombieLogoBmp;
	PausedBmp pausedBmp;
	WinnerBmp winnerBmp;

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

	// Draw title screen
	// TODO: Replace with bitmap
	showText(0, 0, 256, 192, "Really Bad Eggs");

	Hardware::getBottomGfx()->drawBitmap(0, 0, simianZombieLogoBmp.getWidth(), simianZombieLogoBmp.getHeight(), &simianZombieLogoBmp, 0, 0);
	Hardware::getTopBuffer()->buffer();

	SoundPlayer::playTitleMusic();

	while (1) {

		++blanks;

		switch (state) {
			case GAME_STATE_TITLE:
				if (pad.isANewPress() || pad.isStartNewPress()) {
					clearScreens();
					state = GAME_STATE_MENU;
				}
				break;

			case GAME_STATE_MENU:
				menu->iterate();

				if (!menu->isRunning()) {

					srand(blanks);

					switch (menu->getGameType()) {
						case 0:
							gameType = GridRunner::GAME_TYPE_SINGLE_PLAYER;
							blockServer = new BlockServer(1, menu->getColours());
							break;
						case 1:
							gameType = GridRunner::GAME_TYPE_TWO_PLAYER;
							
							blockServer = new BlockServer(2, menu->getColours());
							aiGrid = new Grid(menu->getStartHeight(), 1);
							aiController = new SmartAIController(false);
							aiRunner = new GridRunner(aiController, aiGrid, blockServer, 1, aiRunnerX, gameType, menu->getStartLevel());

							aiController->setGridRunner(aiRunner);
							break;
						case 2:
							gameType = GridRunner::GAME_TYPE_TWO_PLAYER;

							blockServer = new BlockServer(2, menu->getColours());
							aiGrid = new Grid(menu->getStartHeight(), 1);
							aiController = new SmartAIController(true);
							aiRunner = new GridRunner(aiController, aiGrid, blockServer, 1, aiRunnerX, gameType, menu->getStartLevel());

							aiController->setGridRunner(aiRunner);
							break;
					}

					grid = new Grid(menu->getStartHeight(), 0);
					runner = new GridRunner(controller, grid, blockServer, 0, runnerX, gameType, menu->getStartLevel());

					WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
					gfx->drawBitmap(0, 0, background.getWidth(), background.getHeight(), &background, 0, 0);
					Hardware::getTopBuffer()->buffer();

					SoundPlayer::stopMusic();

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
						WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
						
						gfx->drawBitmap(aiRunnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);

						Hardware::getTopBuffer()->buffer();

						state = GAME_STATE_GAME_OVER;
					} else if (aiRunner->isDead() && !runner->isDead()) {
						WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
						
						gfx->drawBitmap(runnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);

						Hardware::getTopBuffer()->buffer();

						state = GAME_STATE_GAME_OVER;
					} else if (aiRunner->isDead() && runner->isDead()) {
						WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

						gfx->drawBitmap(runnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);
						gfx->drawBitmap(aiRunnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);

						Hardware::getTopBuffer()->buffer();

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
					WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

					gfx->drawFilledRect(runnerX, 0, runnerWidth, runnerHeight, woopsiRGB(0, 0, 0));
					gfx->drawBitmap(runnerX, (runnerHeight - 16) / 2, pausedBmp.getWidth(), pausedBmp.getHeight(), &pausedBmp, 0, 0);

					if (aiGrid != NULL) {
						gfx->drawFilledRect(aiRunnerX, 0, runnerWidth, runnerHeight, woopsiRGB(0, 0, 0));
						gfx->drawBitmap(aiRunnerX, (runnerHeight - 16) / 2, pausedBmp.getWidth(), pausedBmp.getHeight(), &pausedBmp, 0, 0);
					}

					Hardware::getTopBuffer()->buffer();

					SoundPlayer::playPause();

					state = GAME_STATE_PAUSED;
				}

				break;
			
			case GAME_STATE_PAUSED:
				if (pad.isStartNewPress()) {

					// Unpause
					state = GAME_STATE_ACTIVE;
				} else if (pad.isSelectNewPress()) {

					// Quit to menu
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

					clearScreens();

					SoundPlayer::playTitleMusic();

					state = GAME_STATE_MENU;
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

					clearScreens();

					SoundPlayer::playTitleMusic();

					state = GAME_STATE_MENU;
				}
				break;
		}

		Hardware::waitForVBlank();
	}

	delete menu;
	delete controller;

	SoundPlayer::shutdown();
	Hardware::shutdown();
	return 0;
}
