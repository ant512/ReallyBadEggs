#ifndef _GRID_H_
#define _GRID_H_

#include <graphics.h>
#include <nds.h>

class Grid {
public:

	static const s32 GRID_WIDTH = 6;
	static const s32 GRID_HEIGHT = 12;

	Grid();
	~Grid();

	void render(WoopsiGfx::Graphics* gfx);
	void iterate();

private:
	u8* _data;
};

#endif
