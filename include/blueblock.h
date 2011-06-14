#ifndef _BLUE_BLOCK_H_
#define _BLUE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "blueblockbmp.h"

class BlueBlock : public NormalBlock {
public:
	BlueBlock() : NormalBlock(woopsiRGB(0, 0, 31)) {
		_bitmaps[CONNECTION_NONE] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP] = new BlueBlockBmp();
		_bitmaps[CONNECTION_BOTTOM] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
	};

	~BlueBlock() { };
};

#endif
