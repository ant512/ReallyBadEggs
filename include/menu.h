#ifndef _MENU_H_
#define _MENU_H_

#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "game.h"
#include "gamefont.h"
#include "menubackgroundbmp.h"
#include "menuset.h"
#include "menuoption.h"
#include "soundplayer.h"

class Menu {
public:
	Menu();

	~Menu();

	void render();

	void drawBackground();

	void reset();

	void iterate();

	s32 getGameType() const;

	s32 getStartLevel() const;

	s32 getStartHeight() const;

	s32 getColours() const;

	bool isRunning() const;

	void setActiveMenu(s32 menu);

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
