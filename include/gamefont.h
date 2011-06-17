#ifndef _GAMEFONT_H_
#define _GAMEFONT_H_

#include <packedfont1.h>

/**
 * GameFont font.
 */
class GameFont : public WoopsiGfx::PackedFont1 {
public:
	/**
	 * Constructor.
	 */
	GameFont();
};

#endif
