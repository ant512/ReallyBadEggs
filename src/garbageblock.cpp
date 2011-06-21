#include "garbageblock.h"
#include "garbageblockbmp.h"

GarbageBlock::GarbageBlock() : BlockBase(COLOUR) {
	_bitmaps[CONNECTION_NONE] = new GarbageBlockBmp();

	_explodingAnim->addFrame(&_explodeBmp1, 0);
	_explodingAnim->addFrame(&_explodeBmp2, 0);
	_explodingAnim->addFrame(&_explodeBmp3, 0);
	_explodingAnim->addFrame(&_explodeBmp4, 0);
	_explodingAnim->addFrame(&_explodeBmp5, 0);
	_explodingAnim->addFrame(&_explodeBmp6, 0);
}

GarbageBlock::~GarbageBlock() {

}
