#include "game.h"

Game::Game() {

};

Game::~Game() {

};

void Game::main() {

	s32 p1RunnerX = Grid::BLOCK_SIZE;
	s32 p2RunnerX = SCREEN_WIDTH - (Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - Grid::BLOCK_SIZE;
	s32 runnerWidth = Grid::GRID_WIDTH * Grid::BLOCK_SIZE;
	s32 runnerHeight = Grid::GRID_HEIGHT * Grid::BLOCK_SIZE;
	GameState state = GAME_STATE_TITLE;

	Menu* menu = new Menu();

	TwoPlayerBgBmp backgroundBmp;
	StatsBackgroundBmp statsBackgroundBmp;
	SimianZombieLogoBmp simianZombieLogoBmp;
	TitleBmp titleBmp;
	PausedBmp pausedBmp;
	WinnerBmp winnerBmp;

	BlockFactory* blockFactory = NULL;

	// Player 1
	Grid* p1Grid = NULL;
	ControllerBase* p1Controller = NULL;
	GridRunner* p1Runner = NULL;
	
	// Player 2
	Grid* p2Grid = NULL;
	ControllerBase* p2Controller = NULL;
	GridRunner* p2Runner = NULL;

	const Pad& pad = Hardware::getPad();

	Scroller* scroller = new Scroller(".... Really Bad Eggs is a remake of Super "
									  "Foul Egg, a Commodore Amiga version of "
									  "Puyo Puyo.  The original was released as "
									  "an Amiga Power coverdisk in 1995 "
									  ".... Connect eggs into chains of 4 or "
									  "more by moving and rotating them as they "
									  "fall "
									  ".... Really Bad Eggs (c) 2011 Antony "
									  "Dzeryn "
									  ".... Original game (c) 1995 Impact "
									  "Productions "
									  ".... See simianzombie.com for other "
									  "projects "
									  ".... Really Bad Eggs was written "
									  "using WoopsiGfx, a 2D graphics library "
									  "that includes features such as DMA "
									  "acceleration, a set of 2D drawing "
									  "routines, animation, UTF-8 strings and "
									  "fonts for rendering text .... "
									  );

	s32 blanks = 0;

	Hardware::getTopGfx()->drawBitmap(0, 0, titleBmp.getWidth(), titleBmp.getHeight(), &titleBmp, 0, 0);

	Hardware::getBottomGfx()->drawBitmap(0, 0, simianZombieLogoBmp.getWidth(), simianZombieLogoBmp.getHeight(), &simianZombieLogoBmp, 0, 0);
	Hardware::getTopBuffer()->buffer();

	SoundPlayer::playTitleMusic();

	while (1) {

		++blanks;

		switch (state) {
			case GAME_STATE_TITLE:
				if (pad.isANewPress() || pad.isStartNewPress()) {
					Hardware::getBottomGfx()->drawBitmap(0, 0, titleBmp.getWidth(), titleBmp.getHeight(), &titleBmp, 0, 0);

					menu->reset();
					state = GAME_STATE_MENU;
				}
				break;

			case GAME_STATE_MENU:
				menu->iterate();

				scroller->render(184, Hardware::getBottomGfx());

				if (!menu->isRunning()) {

					srand(blanks);

					switch (menu->getGameType()) {
						case GAME_TYPE_PRACTICE:
							blockFactory = new BlockFactory(1, menu->getColours());
							p1Controller = new SinglePlayerController();
							break;

						case GAME_TYPE_EASY:
							blockFactory = new BlockFactory(2, menu->getColours());
							p1Controller = new SinglePlayerController();

							p2Grid = new Grid(menu->getHeight(), 1);
							p2Controller = new AIController(EASY_AI_HESITATION);
							p2Runner = new GridRunner(p2Controller, p2Grid, blockFactory, 1, p2RunnerX, menu->getSpeed());

							((AIController*)p2Controller)->setGridRunner(p2Runner);
							break;

						case GAME_TYPE_MEDIUM:
							blockFactory = new BlockFactory(2, menu->getColours());
							p1Controller = new SinglePlayerController();

							p2Grid = new Grid(menu->getHeight(), 1);
							p2Controller = new AIController(MEDIUM_AI_HESITATION);
							p2Runner = new GridRunner(p2Controller, p2Grid, blockFactory, 1, p2RunnerX, menu->getSpeed());

							((AIController*)p2Controller)->setGridRunner(p2Runner);
							break;

						case GAME_TYPE_HARD:
							blockFactory = new BlockFactory(2, menu->getColours());
							p1Controller = new SinglePlayerController();

							p2Grid = new Grid(menu->getHeight(), 1);
							p2Controller = new AIController(HARD_AI_HESITATION);
							p2Runner = new GridRunner(p2Controller, p2Grid, blockFactory, 1, p2RunnerX, menu->getSpeed());

							((AIController*)p2Controller)->setGridRunner(p2Runner);
							break;
						
						case GAME_TYPE_2_PLAYER:
							blockFactory = new BlockFactory(2, menu->getColours());
							p1Controller = new Player1Controller();

							p2Grid = new Grid(menu->getHeight(), 1);
							p2Controller = new Player2Controller();
							p2Runner = new GridRunner(p2Controller, p2Grid, blockFactory, 1, p2RunnerX, menu->getSpeed());
							break;
					}

					p1Grid = new Grid(menu->getHeight(), 0);
					p1Runner = new GridRunner(p1Controller, p1Grid, blockFactory, 0, p1RunnerX, menu->getSpeed());

					WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
					gfx->drawBitmap(0, 0, backgroundBmp.getWidth(), backgroundBmp.getHeight(), &backgroundBmp, 0, 0);

					// Fill second player grid with blank blocks if single
					// player mode is selected
					if (menu->getGameType() == 0) {
						BlankBlockBmp blankBlockBmp;

						for (s32 x = 0; x < Grid::GRID_WIDTH; ++x) {
							for (s32 y = 0; y < Grid::GRID_HEIGHT; ++y) {
								gfx->drawBitmap(p2RunnerX + (x * Grid::BLOCK_SIZE), y * Grid::BLOCK_SIZE, blankBlockBmp.getWidth(), blankBlockBmp.getHeight(), &blankBlockBmp, 0, 0);
							}
						}
					}

					Hardware::getTopBuffer()->buffer();

					// Draw the stats background
					gfx = Hardware::getBottomGfx();
					gfx->drawBitmap(0, 0, statsBackgroundBmp.getWidth(), statsBackgroundBmp.getHeight(), &statsBackgroundBmp, 0, 0);

					// Draw stats
					p1Runner->renderHUD();
					if (p2Runner != NULL) p2Runner->renderHUD();

					SoundPlayer::stopMusic();

					state = GAME_STATE_ACTIVE;
				}

				break;

			case GAME_STATE_ACTIVE:

				// Standard mode
				p1Runner->iterate();

				if (p2Runner == NULL) {
					if (p1Runner->isDead()) {
						SoundPlayer::playDead();

						blanks = 0;
						state = GAME_STATE_GAME_OVER;
					}
				} else {
					p2Runner->iterate();

					if (p1Runner->isDead() && !p2Runner->isDead()) {
						WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
						
						gfx->drawBitmap(p2RunnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);

						Hardware::getTopBuffer()->buffer();

						SoundPlayer::playDead();

						blanks = 0;
						state = GAME_STATE_GAME_OVER;
					} else if (p2Runner->isDead() && !p1Runner->isDead()) {
						WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();
						
						gfx->drawBitmap(p1RunnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);

						Hardware::getTopBuffer()->buffer();

						SoundPlayer::playDead();

						blanks = 0;
						state = GAME_STATE_GAME_OVER;
					} else if (p2Runner->isDead() && p1Runner->isDead()) {
						WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

						gfx->drawBitmap(p1RunnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);
						gfx->drawBitmap(p2RunnerX, (runnerHeight - 16) / 2, winnerBmp.getWidth(), winnerBmp.getHeight(), &winnerBmp, 0, 0);

						Hardware::getTopBuffer()->buffer();

						SoundPlayer::playDead();

						blanks = 0;
						state = GAME_STATE_GAME_OVER;
					}

					if (p1Runner->addIncomingGarbage(p2Runner->getOutgoingGarbageCount())) {
						p2Runner->clearOutgoingGarbageCount();
					}

					if (p2Runner->addIncomingGarbage(p1Runner->getOutgoingGarbageCount())) {
						p1Runner->clearOutgoingGarbageCount();
					}
				}

				if (pad.isStartNewPress()) {
					WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

					gfx->drawFilledRect(p1RunnerX, 0, runnerWidth, runnerHeight, woopsiRGB(0, 0, 0));
					gfx->drawBitmap(p1RunnerX, (runnerHeight - 16) / 2, pausedBmp.getWidth(), pausedBmp.getHeight(), &pausedBmp, 0, 0);

					if (p2Grid != NULL) {
						gfx->drawFilledRect(p2RunnerX, 0, runnerWidth, runnerHeight, woopsiRGB(0, 0, 0));
						gfx->drawBitmap(p2RunnerX, (runnerHeight - 16) / 2, pausedBmp.getWidth(), pausedBmp.getHeight(), &pausedBmp, 0, 0);
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
					delete blockFactory;

					delete p1Runner;
					delete p1Grid;
					delete p1Controller;

					delete p2Runner;
					delete p2Grid;
					delete p2Controller;

					blockFactory = NULL;
					p1Runner = NULL;
					p1Grid = NULL;
					p1Controller = NULL;

					p2Runner = NULL;
					p2Grid = NULL;
					p2Controller = NULL;

					menu->setActiveMenu(1);

					Hardware::getBottomGfx()->drawBitmap(0, 0, titleBmp.getWidth(), titleBmp.getHeight(), &titleBmp, 0, 0);

					SoundPlayer::playTitleMusic();

					menu->reset();

					state = GAME_STATE_MENU;
				}
				break;

			case GAME_STATE_GAME_OVER:

				if (blanks == 200) {
					if (p2Runner != NULL) {
						if (p2Runner->isDead()) {
							SoundPlayer::playWin();
						} else {
							SoundPlayer::playLose();
						}
					}
				}

				if ((blanks > 200) && (pad.isStartNewPress() || pad.isANewPress() || pad.isBNewPress())) {

					delete blockFactory;

					delete p1Runner;
					delete p1Grid;
					delete p1Controller;

					delete p2Runner;
					delete p2Grid;
					delete p2Controller;

					blockFactory = NULL;
					p1Runner = NULL;
					p1Grid = NULL;
					p1Controller = NULL;

					p2Runner = NULL;
					p2Grid = NULL;
					p2Controller = NULL;

					menu->setActiveMenu(1);

					Hardware::getBottomGfx()->drawBitmap(0, 0, titleBmp.getWidth(), titleBmp.getHeight(), &titleBmp, 0, 0);

					SoundPlayer::playTitleMusic();

					menu->reset();

					state = GAME_STATE_MENU;
				}
				break;
		}

		Hardware::waitForVBlank();
	}

	delete menu;
	delete scroller;
}
