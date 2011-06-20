#include "garbageblock.h"
#include "garbageblockbmp.h"

GarbageBlock::GarbageBlock() : BlockBase(woopsiRGB(9, 9, 9)) {
	_bitmaps[CONNECTION_NONE] = new GarbageBlockBmp();
}

GarbageBlock::~GarbageBlock() {

}
