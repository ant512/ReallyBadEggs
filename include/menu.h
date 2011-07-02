#ifndef _MENU_H_
#define _MENU_H_

#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "gamefont.h"
#include "menubackgroundbmp.h"
#include "menuset.h"
#include "menuoption.h"
#include "soundplayer.h"

class Menu {
public:
	Menu() {
		_activeMenu = 0;

		_gameTypeMenu = new MenuSet(52, 100, 72, 4, 1, 0, "Game Type");
		_gameTypeMenu->addOption("Practice", 0);
		_gameTypeMenu->addOption("Easy", 1);
		_gameTypeMenu->addOption("Medium" , 2);
		_gameTypeMenu->addOption("Hard", 3);

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
	};

	~Menu() {
		delete _gameTypeMenu;
		delete _startLevelMenu;
		delete _startHeightMenu;
		delete _coloursMenu;
	};

	void render() {

		WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

		if (_activeMenu == 0) {
			_gameTypeMenu->render(true, &_font, gfx);
		} else {
			_startLevelMenu->render(_activeMenu == 1, &_font, gfx);
			_startHeightMenu->render(_activeMenu == 2, &_font, gfx);
			_coloursMenu->render(_activeMenu == 3, &_font, gfx);
		}
	};

	void drawBackground() {
		Hardware::getTopGfx()->drawBitmap(0, 0, _backgroundBmp.getWidth(), _backgroundBmp.getHeight(), &_backgroundBmp, 0, 0);
		Hardware::getTopBuffer()->buffer();
	};

	void reset() {
		drawBackground();
		_activeMenu = 0;
	};

	void iterate() {
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
	};

	s32 getGameType() const {
		return _gameTypeMenu->getSelectedValue();
	};

	s32 getStartLevel() const {
		return _startLevelMenu->getSelectedValue();
	};

	s32 getStartHeight() const {
		return _startHeightMenu->getSelectedValue();
	};

	s32 getColours() const {
		return _coloursMenu->getSelectedValue();
	};

	bool isRunning() const {
		return _activeMenu < 4;
	};

	void setActiveMenu(s32 menu) {
		_activeMenu = menu;
	};

private:
	MenuSet* _gameTypeMenu;
	MenuSet* _startLevelMenu;
	MenuSet* _startHeightMenu;
	MenuSet* _coloursMenu;

	s32 _activeMenu;

	GameFont _font;
	MenuBackgroundBmp _backgroundBmp;
};

#endif
