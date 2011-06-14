#ifndef _RED_BLOCK_H_
#define _RED_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "redblockbmp.h"
#include "redblockbottombmp.h"
#include "redblocktopbmp.h"
#include "redblocktopbottombmp.h"
#include "redblockleftbmp.h"
#include "redblockrightbmp.h"
#include "redblocktopleftbmp.h"
#include "redblocktoprightbmp.h"
#include "redblockbottomrightbmp.h"
#include "redblockbottomleftbmp.h"
#include "redblockleftrightbmp.h"
#include "redblocktopleftbottombmp.h"
#include "redblocktoprightbottombmp.h"
#include "redblocktopleftrightbmp.h"
#include "redblocktopleftrightbottombmp.h"
#include "redblockbottomleftrightbmp.h"

class RedBlock : public NormalBlock {
public:
	RedBlock() : NormalBlock(woopsiRGB(31, 0, 0)) {
		_bitmaps[CONNECTION_NONE] = new RedBlockBmp();
		_bitmaps[CONNECTION_TOP] = new RedBlockTopBmp();
		_bitmaps[CONNECTION_BOTTOM] = new RedBlockBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new RedBlockTopBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new RedBlockTopLeftBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new RedBlockTopRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new RedBlockTopLeftRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new RedBlockTopLeftBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new RedBlockTopRightBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new RedBlockTopLeftRightBottomBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new RedBlockBottomLeftBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new RedBlockBottomRightBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new RedBlockBottomLeftRightBmp();
		_bitmaps[CONNECTION_LEFT] = new RedBlockLeftBmp();
		_bitmaps[CONNECTION_RIGHT] = new RedBlockRightBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new RedBlockLeftRightBmp();
	};

	~RedBlock() { };
};

#endif
