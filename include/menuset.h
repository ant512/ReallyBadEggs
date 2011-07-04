#ifndef _MENU_SET_H_
#define _MENU_SET_H_

#include <fontbase.h>
#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>
#include <woopsistring.h>

#include "menuoption.h"
#include "soundplayer.h"

/**
 * A set of options that the player can choose from.  Options are arranged in a
 * grid.
 */
class MenuSet {
public:

	/**
	 * Constructor.
	 * @param y The y co-ordinate of the menu set.  The x co-ordinate is
	 * calculated automatically based on the size of the screen and the width of
	 * the menu set such that the menu set is centred.
	 * @param width The width of the menu set.
	 * @param height The height of the menu set.
	 * @param rows The number of rows in the menu set.
	 * @param columns The number of columns in the menu set.
	 * @param value The value of the menu set (a unique ID).
	 * @param title The title of the menu set.
	 */
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

	/**
	 * Destructor.
	 */
	~MenuSet() {
		for (s32 i = 0; i < _options.size(); ++i) {
			delete _options[i];
		}
	};

	/**
	 * Adds a new options to the menu set.
	 * @param text The text of the option.
	 * @param value The value of the option.
	 */
	void addOption(const WoopsiGfx::WoopsiString& text, s32 value) {
		_options.push_back(new MenuOption(text, value));
	};

	/**
	 * Draws the menu set.
	 * @param active True if the menu is active; false if not.
	 * @param font The font to use to render text.
	 * @param gfx The graphics object to draw to.
	 */
	void render(bool active, WoopsiGfx::FontBase* font, WoopsiGfx::Graphics* gfx) {

		u16 titleColour = active ? woopsiRGB(31, 31, 31) : woopsiRGB(9, 9, 9);

		gfx->drawText((SCREEN_WIDTH - font->getStringWidth(_title)) / 2, _y, font, _title, 0, _title.getLength(), titleColour);

		s32 optionWidth = _width / _columns;
		s32 optionHeight = _height / _rows;

		s32 optionX = 0;
		s32 optionY = 0;

		s32 optionIndex = 0;

		s32 x = (SCREEN_WIDTH - _width) / 2;

		gfx->drawFilledRect(x + 4, _y + (font->getHeight() * 2) + 4, _columns * optionWidth, _rows * optionHeight, woopsiRGB(0, 0, 0));

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

	/**
	 * Runs a single iteration of the menu set logic.  Reads the state of the
	 * input controls to determine where to move the selection to.
	 */
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

	/**
	 * Sets the selected option to the supplied index.
	 * @param index The new selected index.
	 */
	void setSelectedOption(s32 index) {

		s32 newValue = index;

		if (newValue < 0) {
			newValue = 0;
		} else if (newValue >= _options.size()) {
			newValue = _options.size() - 1;
		}

		if (_selectedOptionIndex != newValue) {
			_selectedOptionIndex = newValue;
			SoundPlayer::playMenuMove();
		}
	};

	/**
	 * Gets the value of the selected option.
	 * @return The value of the selected option.
	 */
	s32 getSelectedValue() const {
		return _options[_selectedOptionIndex]->getValue();
	};

private:
	s32 _y;								/**< The y co-ordinate of the menu set. */
	s32 _width;							/**< The width of the menu set. */
	s32 _height;						/**< The height of the menu set. */
	s32 _selectedOptionIndex;			/**< The index of the selected option. */
	s32 _rows;							/**< The number of rows in the menu set. */
	s32 _columns;						/**< The number of columns in the menu set. */
	s32 _value;							/**< The value of the menu set. */
	WoopsiArray<MenuOption*> _options;	/**< The list of options in the menu set. */
	WoopsiGfx::WoopsiString _title;		/**< The title of the menu set. */
};

#endif
