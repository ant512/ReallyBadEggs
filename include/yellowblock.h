#ifndef _YELLOW_BLOCK_H_
#define _YELLOW_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "yellowblockbmp.h"

class YellowBlock : public NormalBlock {
public:
	YellowBlock() : NormalBlock(woopsiRGB(31, 31, 0)) {
		_bitmaps[CONNECTION_NONE] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
	};

	~YellowBlock() { };
};

#endif
