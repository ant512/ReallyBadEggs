#ifndef _MENU_OPTION_H_
#define _MENU_OPTION_H_

#include <fontbase.h>
#include <graphics.h>
#include <nds.h>
#include <woopsistring.h>

#include "hardware.h"
#include "pad.h"

/**
 * An option within a MenuSet object.
 */
class MenuOption {
public:

	/**
	 * Constructor.
	 * @param text The text of the option.
	 * @param value The value of the option.
	 */
	MenuOption(const WoopsiGfx::WoopsiString& text, s32 value) {
		_text = text;
		_value = value;
	};

	/**
	 * Destructor.
	 */
	~MenuOption() { };

	/**
	 * Draws the option.
	 * @param x The x co-ordinate to draw to.
	 * @param y The y co-ordinate to draw to.
	 * @param width The width of the option.
	 * @param height The height of the option.
	 * @param selected True if the option is selected; false if not.
	 * @param active True if the option is active; false if not.
	 * @param font The font to draw text with.
	 * @param gfx The Graphics object to draw with.
	 */
	void render(s32 x, s32 y, s32 width, s32 height, bool selected, bool active, WoopsiGfx::FontBase* font, WoopsiGfx::Graphics* gfx) {

		u16 bgColour = selected ? (active ? woopsiRGB(31, 31, 0) : woopsiRGB(4, 4, 4)) : woopsiRGB(0, 0, 0);
		u16 textColour = selected ? woopsiRGB(0, 0, 0) : (active ? woopsiRGB(31, 31, 31) : woopsiRGB(9, 9, 9));
		u16 outlineColour = active ? woopsiRGB(31, 31, 31) : woopsiRGB(9, 9, 9);

		s32 textX = x + ((width - font->getStringWidth(_text)) / 2);
		s32 textY = y + ((height - font->getHeight()) / 2);

		gfx->drawRect(x, y, width, height, outlineColour);
		gfx->drawFilledRect(x + 1, y + 1, width - 2, height - 2, bgColour);
		gfx->drawText(textX, textY, font, _text, 0, _text.getLength(), textColour);
	};

	/**
	 * Gets the value of the option.
	 * @return The value of the option.
	 */
	s32 getValue() const {
		return _value;
	};

private:
	WoopsiGfx::WoopsiString _text;		/**< The text of the option. */
	s32 _value;							/**< The value of the option. */
};

#endif
