#ifndef _MENU_SET_H_
#define _MENU_SET_H_

#include <fontbase.h>
#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "menuoption.h"

class MenuSet {
public:
	MenuSet(s32 y, s32 width, s32 height, s32 rows, s32 columns, s32 value, const WoopsiGfx::WoopsiString& title) {
		_y = y;
		_width = width;
		_height = height;
		_rows = rows;
		_columns = columns;
		_value = value;
		_title = title;
		_selectedOptionIndex = 0;
	};

	~MenuSet() {
		for (s32 i = 0; i < _options.size(); ++i) {
			delete _options[i];
		}
	};

	void addOption(const WoopsiGfx::WoopsiString& text, s32 value) {
		_options.push_back(new MenuOption(text, value));
	};

	void render(bool active, WoopsiGfx::FontBase* font, WoopsiGfx::Graphics* gfx) {

		u16 titleColour = active ? woopsiRGB(31, 31, 31) : woopsiRGB(9, 9, 9);

		gfx->drawText((SCREEN_WIDTH - font->getStringWidth(_title)) / 2, _y, font, _title, 0, _title.getLength(), titleColour);

		s32 optionWidth = _width / _columns;
		s32 optionHeight = _height / _rows;

		s32 optionX = 0;
		s32 optionY = 0;

		s32 optionIndex = 0;

		s32 x = (SCREEN_WIDTH - _width) / 2;

		for (s32 row = 0; row < _rows; ++row) {

			optionY = _y + (optionHeight * row) + (font->getHeight() * 2);
			
			for (s32 col = 0; col < _columns; ++col) {

				optionX = x + (optionWidth * col);

				if (optionIndex < _options.size()) {
					_options[optionIndex]->render(optionX, optionY, optionWidth, optionHeight, _selectedOptionIndex == optionIndex, active, font, gfx);
				} else {
					gfx->drawFilledRect(optionX, optionY, optionWidth, optionHeight, woopsiRGB(0, 0, 0));
				}

				++optionIndex;
			}
		}
	};

	void iterate() {
		const Pad& pad = Hardware::getPad();

		if (pad.isLeftNewPress() || pad.isLeftRepeat()) {
			setSelectedOption(_selectedOptionIndex - 1);
		} else if (pad.isUpNewPress() || pad.isUpRepeat()) {
			if (_selectedOptionIndex - _columns >= 0) {
				setSelectedOption(_selectedOptionIndex - _columns);
			}
		} else if (pad.isRightNewPress() || pad.isRightRepeat()) {
			setSelectedOption(_selectedOptionIndex + 1);
		} else if (pad.isDownNewPress() || pad.isDownRepeat()) {
			if (_selectedOptionIndex + _columns < _options.size()) {
				setSelectedOption(_selectedOptionIndex + _columns);
			}
		}
	};

	void setSelectedOption(s32 index) {
		_selectedOptionIndex = index;

		if (_selectedOptionIndex < 0) {
			_selectedOptionIndex = 0;
		} else if (_selectedOptionIndex >= _options.size()) {
			_selectedOptionIndex = _options.size() - 1;
		}
	};

	s32 getSelectedValue() const {
		return _options[_selectedOptionIndex]->getValue();
	};

private:
	s32 _y;
	s32 _width;
	s32 _height;
	s32 _selectedOptionIndex;
	s32 _rows;
	s32 _columns;
	s32 _value;
	WoopsiArray<MenuOption*> _options;
	WoopsiGfx::WoopsiString _title;
};

#endif
