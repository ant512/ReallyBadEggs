#ifndef _GAME_H_
#define _GAME_H_

#include <nds.h>

#include <graphics.h>
#include <pad.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "blankblockbmp.h"
#include "blockfactory.h"
#include "gamefont.h"
#include "grid.h"
#include "gridrunner.h"
#include "hardware.h"
#include "menu.h"
#include "pausedbmp.h"
#include "singleplayercontroller.h"
#include "player1controller.h"
#include "player2controller.h"
#include "scroller.h"
#include "simianzombielogobmp.h"
#include "statsbackgroundbmp.h"
#include "aicontroller.h"
#include "soundplayer.h"
#include "twoplayerbgbmp.h"
#include "winnerbmp.h"

/**
 * Main game class.
 */
class Game {
public:

	/**
	 * List of all game types.
	 */
	enum GameType {
		GAME_TYPE_PRACTICE = 0,		/**< Single player. */
		GAME_TYPE_EASY = 1,			/**< Player vs easy CPU. */
		GAME_TYPE_MEDIUM = 2,		/**< Player vs medium CPU. */
		GAME_TYPE_HARD = 3,			/**< Player vs hard CPU. */
		GAME_TYPE_2_PLAYER = 4		/**< Player vs player. */
	};

	/**
	 * Constructor.
	 */
	Game();

	/**
	 * Destructor.
	 */
	~Game();

	/**
	 * Main game method.
	 */
	void main();

private:
	static const s32 HARD_AI_HESITATION = 0;		/**< Amount of hesitation by hard CPU. */
	static const s32 MEDIUM_AI_HESITATION = 2;		/**< Amount of hesitation by medium CPU. */
	static const s32 EASY_AI_HESITATION = 4;		/**< Amount of hesitation by easy CPU. */

	/**
	 * List of all possible states the game can be in.
	 */
	enum GameState {
		GAME_STATE_TITLE = 0,						/**< Title screen active. */
		GAME_STATE_MENU = 1,						/**< Menu screen active. */
		GAME_STATE_ACTIVE = 2,						/**< Game running. */
		GAME_STATE_PAUSED = 3,						/**< Game paused. */
		GAME_STATE_GAME_OVER = 4					/**< Game ended. */
	};

	/**
	 * Clears both DS screens to black.
	 */
	void clearScreens();

	// TODO: Delete this when title screen is in place.
	void showText(s32 x, s32 y, s32 width, s32 height, const WoopsiGfx::WoopsiString& text);
};

#endif
