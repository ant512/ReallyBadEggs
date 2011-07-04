#include "game.h"
#include "menu.h"

Menu::Menu() {
	_activeMenu = 0;

	_gameTypeMenu = new MenuSet(43, 100, 90, 5, 1, 0, "Game Type");
	_gameTypeMenu->addOption("Practice", Game::GAME_TYPE_PRACTICE);
	_gameTypeMenu->addOption("Easy", Game::GAME_TYPE_EASY);
	_gameTypeMenu->addOption("Medium" , Game::GAME_TYPE_MEDIUM);
	_gameTypeMenu->addOption("Hard", Game::GAME_TYPE_HARD);
	_gameTypeMenu->addOption("2 Player", Game::GAME_TYPE_2_PLAYER);

	_startLevelMenu = new MenuSet(23, 70, 28, 2, 5, 1, "Speed");
	_startLevelMenu->addOption("0", 0);
	_startLevelMenu->addOption("1", 1);
	_startLevelMenu->addOption("2", 2);
	_startLevelMenu->addOption("3", 3);
	_startLevelMenu->addOption("4", 4);
	_startLevelMenu->addOption("5", 5);
	_startLevelMenu->addOption("6", 6);
	_startLevelMenu->addOption("7", 7);
	_startLevelMenu->addOption("8", 8);
	_startLevelMenu->addOption("9", 9);

	_startHeightMenu = new MenuSet(81, 56, 28, 2, 3, 2, "Height");
	_startHeightMenu->addOption("0", 0);
	_startHeightMenu->addOption("1", 1);
	_startHeightMenu->addOption("2", 2);
	_startHeightMenu->addOption("3", 3);
	_startHeightMenu->addOption("4", 4);
	_startHeightMenu->addOption("5", 5);

	_coloursMenu = new MenuSet(139, 42, 14, 1, 3, 3, "Egg Colours");
	_coloursMenu->addOption("4", 4);
	_coloursMenu->addOption("5", 5);
	_coloursMenu->addOption("6", 6);
}

Menu::~Menu() {
	delete _gameTypeMenu;
	delete _startLevelMenu;
	delete _startHeightMenu;
	delete _coloursMenu;
}

void Menu::render() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	if (_activeMenu == 0) {
		_gameTypeMenu->render(true, &_font, gfx);
	} else {
		_startLevelMenu->render(_activeMenu == 1, &_font, gfx);
		_startHeightMenu->render(_activeMenu == 2, &_font, gfx);
		_coloursMenu->render(_activeMenu == 3, &_font, gfx);
	}
}

void Menu::drawBackground() {
	Hardware::getTopGfx()->drawBitmap(0, 0, _backgroundBmp.getWidth(), _backgroundBmp.getHeight(), &_backgroundBmp, 0, 0);
	Hardware::getTopBuffer()->buffer();
}

void Menu::reset() {
	drawBackground();
	_activeMenu = 0;
}

void Menu::iterate() {
	const Pad& pad = Hardware::getPad();

	if (pad.isANewPress()) {
		if (_activeMenu < 4) {
			++_activeMenu;

			if (_activeMenu == 1) {
				drawBackground();
			}
		}

		SoundPlayer::playMenuSelect();

	} else if (pad.isBNewPress()) {
		if (_activeMenu > 0) {
			--_activeMenu;

			if (_activeMenu == 0) {
				drawBackground();
			}

			SoundPlayer::playMenuBack();
		}

	} else if (pad.isStartNewPress()) {
		// Jump to the end of the menu
		_activeMenu = 4;

		SoundPlayer::playMenuSelect();
	}

	switch (_activeMenu) {
		case 0:
			_gameTypeMenu->iterate();
			break;
		case 1:
			_startLevelMenu->iterate();
			break;
		case 2:
			_startHeightMenu->iterate();
			break;
		case 3:
			_coloursMenu->iterate();
			break;
	}

	render();
}

s32 Menu::getGameType() const {
	return _gameTypeMenu->getSelectedValue();
}

s32 Menu::getStartLevel() const {
	return _startLevelMenu->getSelectedValue();
}

s32 Menu::getStartHeight() const {
	return _startHeightMenu->getSelectedValue();
}

s32 Menu::getColours() const {
	return _coloursMenu->getSelectedValue();
}

bool Menu::isRunning() const {
	return _activeMenu < 4;
}

void Menu::setActiveMenu(s32 menu) {
	_activeMenu = menu;
}
