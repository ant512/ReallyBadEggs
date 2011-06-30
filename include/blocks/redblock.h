#ifndef _RED_BLOCK_H_
#define _RED_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "redblockbmp.h"
#include "redblockbottombmp.h"
#include "redblockbottomleftbmp.h"
#include "redblockbottomleftrightbmp.h"
#include "redblockbottomrightbmp.h"
#include "redblockbounce1bmp.h"
#include "redblockbounce2bmp.h"
#include "redblockexplodebmp.h"
#include "redblockleftbmp.h"
#include "redblockleftrightbmp.h"
#include "redblockrightbmp.h"
#include "redblocktopbmp.h"
#include "redblocktopbottombmp.h"
#include "redblocktopleftbmp.h"
#include "redblocktopleftbottombmp.h"
#include "redblocktopleftrightbmp.h"
#include "redblocktopleftrightbottombmp.h"
#include "redblocktoprightbmp.h"
#include "redblocktoprightbottombmp.h"

#include "blockexplode1bmp.h"
#include "blockexplode2bmp.h"
#include "blockexplode3bmp.h"
#include "blockexplode4bmp.h"
#include "blockexplode5bmp.h"

/**
 * A red block.
 */
class RedBlock : public NormalBlock {
public:

	/**
	 * Constructor.
	 */
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

		_explodingAnim->addFrame(&_explodeBmp1, 0);
		_explodingAnim->addFrame(&_explodeBmp2, 0);
		_explodingAnim->addFrame(&_explodeBmp3, 0);
		_explodingAnim->addFrame(&_explodeBmp4, 0);
		_explodingAnim->addFrame(&_explodeBmp5, 0);
		_explodingAnim->addFrame(&_explodeBmp6, 0);

		_landingAnim->addFrame(&_bounceBmp1, 0);
		_landingAnim->addFrame(&_bounceBmp2, 0);
		_landingAnim->addFrame(&_bounceBmp1, 0);
		_landingAnim->addFrame(&_bounceBmp3, 0);
		_landingAnim->addFrame(&_bounceBmp1, 0);
	};

	/**
	 * Destructor.
	 */
	~RedBlock() { };

private:
	RedBlockExplodeBmp _explodeBmp1;	/**< First explosion bitmap. */
	BlockExplode1Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	BlockExplode2Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	BlockExplode3Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	BlockExplode4Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	BlockExplode5Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */

	RedBlockBounce1Bmp _bounceBmp1;		/**< First block bounce bitmap. */
	RedBlockBounce2Bmp _bounceBmp2;		/**< Second block bounce bitmap. */
	RedBlockBmp _bounceBmp3;			/**< Third block bounce bitmap. */
};

#endif
