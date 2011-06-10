#ifndef _PAD_H_
#define _PAD_H_

#include <nds.h>

/**
 * Abstraction of the DS' pad state to aid porting.
 */
class Pad {
public:

	static const s32 PAD_REPEAT_TIME = 10;

	Pad() {
		_up = 0;
		_down = 0;
		_left = 0;
		_right = 0;
		_a = 0;
		_b = 0;
		_x = 0;
		_y = 0;
		_l = 0;
		_r = 0;
		_start = 0;
		_select = 0;
	};

	~Pad() { };

	inline bool isUpNewPress() const { return _up == 1; };
	inline bool isDownNewPress() const { return _down == 1; };
	inline bool isLeftNewPress() const { return _left == 1; };
	inline bool isRightNewPress() const { return _right == 1; };
	inline bool isANewPress() const { return _a == 1; };
	inline bool isBNewPress() const { return _b == 1; };
	inline bool isXNewPress() const { return _x == 1; };
	inline bool isYNewPress() const { return _y == 1; };
	inline bool isLNewPress() const { return _l == 1; };
	inline bool isRNewPress() const { return _r == 1; };
	inline bool isStartNewPress() const { return _start == 1; };
	inline bool isSelectNewPress() const { return _select == 1; };

	inline bool isUpHeld() const { return _up > 0; };
	inline bool isDownHeld() const { return _down > 0; };
	inline bool isLeftHeld() const { return _left > 0; };
	inline bool isRightHeld() const { return _right > 0; };
	inline bool isAHeld() const { return _a > 0; };
	inline bool isBHeld() const { return _b > 0; };
	inline bool isXHeld() const { return _x > 0; };
	inline bool isYHeld() const { return _y > 0; };
	inline bool isLHeld() const { return _l > 0; };
	inline bool isRHeld() const { return _r > 0; };
	inline bool isStartHeld() const { return _start > 0; };
	inline bool isSelectHeld() const { return _select > 0; };

	inline bool isUpRepeat() const { return _up > 0 && _up % PAD_REPEAT_TIME == 0; };
	inline bool isDownRepeat() const { return _down > 0 && _down % PAD_REPEAT_TIME == 0; };
	inline bool isLeftRepeat() const { return _left > 0 && _left % PAD_REPEAT_TIME == 0; };
	inline bool isRightRepeat() const { return _right > 0 && _right % PAD_REPEAT_TIME == 0; };
	inline bool isARepeat() const { return _a > 0 && _a % PAD_REPEAT_TIME == 0; };
	inline bool isBRepeat() const { return _b > 0 && _b % PAD_REPEAT_TIME == 0; };
	inline bool isXRepeat() const { return _x > 0 && _x % PAD_REPEAT_TIME == 0; };
	inline bool isYRepeat() const { return _y > 0 && _y % PAD_REPEAT_TIME == 0; };
	inline bool isLRepeat() const { return _l > 0 && _l % PAD_REPEAT_TIME == 0; };
	inline bool isRRepeat() const { return _r > 0 && _r % PAD_REPEAT_TIME == 0; };
	inline bool isStartRepeat() const { return _start > 0 && _start % PAD_REPEAT_TIME == 0; };
	inline bool isSelectRepeat() const { return _select > 0 && _select % PAD_REPEAT_TIME == 0; };

	bool isMostRecentDirectionVertical() const {
		if (_up > 0 && (_up < _left || _up < _right)) return true;
		if (_down > 0 && (_down < _left || _down < _right)) return true;
		return false;
	};

	void update() {

#ifndef USING_SDL

		scanKeys();

		s32 pressed = keysDown();	// buttons pressed this loop
		s32 held = keysHeld();		// buttons currently held
		s32 allKeys = pressed | held;

		allKeys & KEY_UP ? _up++ : _up = 0;
		allKeys & KEY_DOWN ? _down++ : _down = 0;
		allKeys & KEY_LEFT ? _left++ : _left = 0;
		allKeys & KEY_RIGHT ? _right++ : _right = 0;
		allKeys & KEY_L ? _l++ : _l = 0;
		allKeys & KEY_R ? _r++ : _r = 0;
		allKeys & KEY_A ? _a++ : _a = 0;
		allKeys & KEY_B ? _b++ : _b = 0;
		allKeys & KEY_X ? _x++ : _x = 0;
		allKeys & KEY_Y ? _y++ : _y = 0;
		allKeys & KEY_START ? _start++ : _start = 0;
		allKeys & KEY_SELECT ? _select++ : _select = 0;

#else

		Uint8* keyState = SDL_GetKeyState(NULL);

		keyState[SDLK_UP] ? _up++ : _up = 0;
		keyState[SDLK_DOWN] ? _down++ : _down = 0;
		keyState[SDLK_LEFT] ? _left++ : _left = 0;
		keyState[SDLK_RIGHT] ? _right : _right = 0;
		keyState[SDLK_a] ? _l++ : _l = 0;
		keyState[SDLK_s] ? _r++ : _r = 0;
		keyState[SDLK_z] ? _a++ : _a = 0;
		keyState[SDLK_x] ? _b++ : _b = 0;
		keyState[SDLK_c] ? _x++ : _x = 0;
		keyState[SDLK_v] ? _y++ : _y = 0;
		keyState[SDLK_d] ? _start++ : _start = 0;
		keyState[SDLK_f] ? _select++ : _select = 0;

#endif
	};

private:

	/**
	 * Each value represents the amount of time (jn VBLs) that the button has
	 * been held.  Therefore:
	 *   0 = not held;
	 *   1 = newly NewPress;
	 *  >1 = held for n frames.
	 */

	s32 _up;		/**< Is up NewPress? */
	s32 _down;		/**< Is down NewPress? */
	s32 _left;		/**< Is left NewPress? */
	s32 _right;		/**< Is right NewPress? */
	s32 _a;			/**< Is a NewPress? */
	s32 _b;			/**< Is b NewPress? */
	s32 _x;			/**< Is x NewPress? */
	s32 _y;			/**< Is y NewPress? */
	s32 _l;			/**< Is l NewPress? */
	s32 _r;			/**< Is r NewPress? */
	s32 _start;		/**< Is start NewPress? */
	s32 _select;	/**< Is select NewPress? */
};

#endif
