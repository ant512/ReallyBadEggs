#ifndef _STYLUS_H_
#define _STYLUS_H_

#include <nds.h>

/**
 * Abstraction of the DS' stylus state to aid porting.
 */
class Stylus {
public:

	static const s32 STYLUS_REPEAT_TIME = 10;

	Stylus() {
		_touch = 0;
		_x = 0;
		_y = 0;
	};

	~Stylus() { };

	inline bool isNewPress() const { return _touch == 1; };
	inline bool isHeld() const { return _touch > 0; };
	inline bool isRepeat() const { return _touch > 0 && _touch % STYLUS_REPEAT_TIME == 0; };

	s16 getX() const { return _x; };
	s16 getY() const { return _y; };

	void update() {

#ifndef USING_SDL

		s32 pressed = keysDown();	// buttons pressed this loop
		s32 held = keysHeld();		// buttons currently held
		s32 allKeys = pressed | held;

		allKeys & KEY_TOUCH ? _touch++ : _touch = -1;

		touchPosition touch;
		touchRead(&touch);

		_x = touch.px;
		_y = touch.py;

#else

		// Read mouse state
		int mouseX;
		int mouseY;
		
		int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		
		// Check buttons
		mouseState & SDL_BUTTON_LEFT ? _stylus.touch++ : _stylus.touch = 0;
		
		_stylus.x = mouseX;
		_stylus.y = mouseY - SCREEN_HEIGHT;
#endif
	};

private:
	s32 _touch;			/**< Stylus has been newly pressed */
	s16 _x;				/**< X co-ord of the stylus */
	s16 _y;				/**< Y co-ord of the stylus */
};

#endif
