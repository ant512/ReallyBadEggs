#include "greyblock.h"
#include "greyblockbmp.h"

GreyBlock::GreyBlock() : BlockBase(woopsiRGB(9, 9, 9)) {
	_bitmaps[CONNECTION_NONE] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP] = new GreyBlockBmp();
	_bitmaps[CONNECTION_BOTTOM] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_LEFT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_RIGHT] = new GreyBlockBmp();
	_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new GreyBlockBmp();
}

GreyBlock::~GreyBlock() {

}
