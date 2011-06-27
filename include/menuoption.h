#ifndef _MENU_OPTION_H_
#define _MENU_OPTION_H_

#include <fontbase.h>
#include <graphics.h>
#include <nds.h>
#include <woopsistring.h>

#include "hardware.h"
#include "pad.h"

class MenuOption {
public:
	MenuOption(const WoopsiGfx::WoopsiString& text, s32 value) {
		_text = text;
		_value = value;
	};

	~MenuOption() { };

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

	s32 getValue() const {
		return _value;
	};

private:
	WoopsiGfx::WoopsiString _text;
	s32 _value;
};

#endif
