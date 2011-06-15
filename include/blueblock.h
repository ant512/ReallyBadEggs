#ifndef _BLUE_BLOCK_H_
#define _BLUE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "blueblockbmp.h"
#include "blueblockbottombmp.h"
//#include "blueblockbottomleftbmp.h"
//#include "blueblockbottomleftrightbmp.h"
#include "blueblockbottomrightbmp.h"
//#include "blueblockbounce1bmp.h"
//#include "blueblockbounce2bmp.h"
#include "blueblockexplodebmp.h"
#include "blueblockleftbmp.h"
//#include "blueblockleftrightbmp.h"
//#include "blueblockrightbmp.h"
#include "blueblocktopbmp.h"
//#include "blueblocktopbottombmp.h"
#include "blueblocktopleftbmp.h"
#include "blueblocktopleftbottombmp.h"
//#include "blueblocktopleftrightbmp.h"
//#include "blueblocktopleftrightbottombmp.h"
#include "blueblocktoprightbmp.h"
//#include "blueblocktoprightbottombmp.h"

#include "blockexplode1bmp.h"
#include "blockexplode2bmp.h"
#include "blockexplode3bmp.h"
#include "blockexplode4bmp.h"
#include "blockexplode5bmp.h"

class BlueBlock : public NormalBlock {
public:
	BlueBlock() : NormalBlock(woopsiRGB(0, 0, 31)) {
		_bitmaps[CONNECTION_NONE] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP] = new BlueBlockTopBmp();
		_bitmaps[CONNECTION_BOTTOM] = new BlueBlockBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new BlueBlockTopLeftBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new BlueBlockTopRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new BlueBlockTopLeftBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new BlueBlockBottomRightBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new BlueBlockLeftBmp();
		_bitmaps[CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();

		_explodingAnim->addFrame(&_explodeBmp1, 0);
		_explodingAnim->addFrame(&_explodeBmp2, 0);
		_explodingAnim->addFrame(&_explodeBmp3, 0);
		_explodingAnim->addFrame(&_explodeBmp4, 0);
		_explodingAnim->addFrame(&_explodeBmp5, 0);
		_explodingAnim->addFrame(&_explodeBmp6, 0);
	};

	~BlueBlock() { };

private:
	BlueBlockExplodeBmp _explodeBmp1;
	BlockExplode1Bmp _explodeBmp2;
	BlockExplode2Bmp _explodeBmp3;
	BlockExplode3Bmp _explodeBmp4;
	BlockExplode4Bmp _explodeBmp5;
	BlockExplode5Bmp _explodeBmp6;
};

#endif
