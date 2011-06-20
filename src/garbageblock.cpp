#include "garbageblock.h"
#include "garbageblockbmp.h"

GarbageBlock::GarbageBlock() : BlockBase(COLOUR) {
	_bitmaps[CONNECTION_NONE] = new GarbageBlockBmp();
}

GarbageBlock::~GarbageBlock() {

}
