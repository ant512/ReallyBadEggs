#ifndef _GAME_H_
#define _GAME_H_

#include <nds.h>

#include <graphics.h>
#include <pad.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "blankblockbmp.h"
#include "blockserver.h"
#include "gamefont.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "menu.h"
#include "pausedbmp.h"
#include "playercontroller.h"
#include "scroller.h"
#include "simianzombielogobmp.h"
#include "statsbackgroundbmp.h"
#include "aicontroller.h"
#include "soundplayer.h"
#include "twoplayerbgbmp.h"
#include "winnerbmp.h"

#include "dumbaicontroller.h"

class Game {
public:

	Game();

	~Game();

	void main();

private:
	static const s32 HARD_AI_HESITATION = 0;
	static const s32 MEDIUM_AI_HESITATION = 2;
	static const s32 EASY_AI_HESITATION = 4;

	enum GameState {
		GAME_STATE_TITLE = 0,
		GAME_STATE_MENU = 1,
		GAME_STATE_ACTIVE = 2,
		GAME_STATE_PAUSED = 3,
		GAME_STATE_GAME_OVER = 4
	};

	void showText(s32 x, s32 y, s32 width, s32 height, const WoopsiGfx::WoopsiString& text);
	void clearScreens();
};

#endif
