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

	void render(s32 x, s32 y, s32 width, s32 height, WoopsiGfx::FontBase* font, WoopsiGfx::Graphics* gfx) {

		u16 bgColour = _selected ? woopsiRGB(31, 31, 0) : woopsiRGB(0, 0, 0);
		u16 textColour = _selected ? woopsiRGB(0, 0, 0) : woopsiRGB(31, 31, 31);

		s32 textX = x + ((width - font->getStringWidth(_text)) / 2);
		s32 textY = y + ((height - font->getHeight()) / 2);

		gfx->drawRect(x, y, width, height, woopsiRGB(31, 31, 31));
		gfx->drawFilledRect(x + 1, y + 1, width - 2, height - 2, bgColour);
		gfx->drawText(textX, textY, font, _text, 0, _text.getLength(), textColour);
	};

	void setSelected(bool selected) {
		_selected = selected;
	};

	s32 getValue() const {
		return _value;
	};

private:
	bool _selected;
	WoopsiGfx::WoopsiString _text;
	s32 _value;
};

#endif
