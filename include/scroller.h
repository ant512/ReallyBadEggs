#ifndef _SCROLLER_H_
#define _SCROLLER_H_

#include <nds.h>
#include <graphics.h>
#include <woopsistring.h>

#include "gamefont.h"

/**
 * Horizontal text scroller.
 */
class Scroller {
public:

	/**
	 * Constructor.
	 * @param text The text to display.
	 */
	Scroller(const WoopsiGfx::WoopsiString& text);

	/**
	 * Destructor.
	 */
	~Scroller() { };

	/**
	 * Renders the scroller at the specified y co-ordinate using the supplied
	 * Graphics object.
	 * @param y The y co-ordinate to render at.
	 * @param gfx The Graphics object to render with.
	 */
	void render(s32 y, WoopsiGfx::Graphics* gfx);

private:
	s32 _offset;					/**< Current scroll amount. */
	s32 _stringWidth;				/**< Width in pixels of the string. */
	GameFont _font;					/**< Font used to render the text. */
	WoopsiGfx::WoopsiString _text;	/**< Text being displayed. */
};

#endif
