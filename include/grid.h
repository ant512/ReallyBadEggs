#ifndef _GRID_H_
#define _GRID_H_

#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>

#include "point.h"

class Grid {
public:

	static const s32 GRID_WIDTH = 6;
	static const s32 GRID_HEIGHT = 12;
	static const s32 CHAIN_LENGTH = 4;

	Grid();
	~Grid();

	void render(WoopsiGfx::Graphics* gfx);
	void iterate();
	void clear();

	u8 getBlockAt(s32 x, s32 y) const;
	void setBlockAt(s32 x, s32 y, u8 block);
	bool isValidCoordinate(s32 x, s32 y) const;

	void getChain(s32 x, s32 y, WoopsiArray<Point>& chain, bool* checkedData);
	void getChains(WoopsiArray<WoopsiArray<Point>*>& chains);

private:
	u8* _data;

};

#endif
