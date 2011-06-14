#include "greyblock.h"
#include "greyblockbmp.h"

GreyBlock::GreyBlock() : BlockBase(woopsiRGB(9, 9, 9)) {
	_bitmaps[CONNECTION_NONE] = new GreyBlockBmp();
}

GreyBlock::~GreyBlock() {

}
