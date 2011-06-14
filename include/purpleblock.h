#ifndef _PURPLE_BLOCK_H_
#define _PURPLE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "purpleblockbmp.h"

class PurpleBlock : public NormalBlock {
public:
	PurpleBlock() : NormalBlock(woopsiRGB(31, 0, 31)) {
		_bitmaps[CONNECTION_NONE] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
	};

	~PurpleBlock() { };
};

#endif
