#ifndef _BLOCK_BASE_H_
#define _BLOCK_BASE_H_

#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

class Grid;

class BlockBase {
public:
	BlockBase(s32 x, s32 y) {
		_x = x;
		_y = y;
	};

	virtual ~BlockBase() { };

	inline s32 getX() const { return _x; };
	inline s32 getY() const { return _y; };

private:
	s32 _x;
	s32 _y;

	WoopsiArray<BitmapBase*> _bitmaps;
};

#endif
