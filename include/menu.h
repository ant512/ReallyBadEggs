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

/**
 * Menu system.  Presents options in a grid.  Similar to the menus used in the
 * original GameBoy Tetris.  All options are hard-coded into the constructor,
 * and selected options are hard-coded as properties.
 */
class Menu {
public:

	/**
	 * Constructor.
	 */
	Menu();

	/**
	 * Destructor.
	 */
	~Menu();

	/**
	 * Draws the menu to the top screen.
	 */
	void render();

	/**
	 * Draws the background bitmap.
	 */
	void drawBackground();

	/**
	 * Resets the menu to the first menu set.
	 */
	void reset();

	/**
	 * Runs a single iteration of the menu logic.
	 */
	void iterate();

	/**
	 * Get the selected game type.
	 * @return The selected game type.
	 */
	s32 getGameType() const;

	/**
	 * Get the selected speed.
	 * @return The selected speed.
	 */
	s32 getSpeed() const;

	/**
	 * Get the selected height.
	 * @return The selected height.
	 */
	s32 getHeight() const;

	/**
	 * Get the selected number of block colours.
	 * @return The selected number of block colours.
	 */
	s32 getColours() const;

	/**
	 * Check if the menu is running or if an option to end the menu has been
	 * selected.
	 * @return True if the menu is running; false if it has ended.
	 */
	bool isRunning() const;

	/**
	 * Sets the active menu.
	 * @param menu The new active menu value.
	 */
	void setActiveMenu(s32 menu);

private:
	MenuSet* _gameTypeMenu;				/**< The game type menu. */
	MenuSet* _speedMenu;				/**< The speed menu. */
	MenuSet* _heightMenu;				/**< The height menu. */
	MenuSet* _coloursMenu;				/**< The block colour count menu. */

	s32 _activeMenu;					/**< Value of the active menu. */

	GameFont _font;						/**< Font used for rendering text. */
	MenuBackgroundBmp _backgroundBmp;	/**< Background bitmap. */
};

#endif
