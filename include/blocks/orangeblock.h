#ifndef _ORANGE_BLOCK_H_
#define _ORANGE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "orangeblockbmp.h"
#include "orangeblockbottombmp.h"
#include "orangeblockbottomleftbmp.h"
#include "orangeblockbottomleftrightbmp.h"
#include "orangeblockbottomrightbmp.h"
#include "orangeblockbounce1bmp.h"
#include "orangeblockbounce2bmp.h"
#include "orangeblockexplodebmp.h"
#include "orangeblockleftbmp.h"
#include "orangeblockleftrightbmp.h"
#include "orangeblockrightbmp.h"
#include "orangeblocktopbmp.h"
#include "orangeblocktopbottombmp.h"
#include "orangeblocktopleftbmp.h"
#include "orangeblocktopleftbottombmp.h"
#include "orangeblocktopleftrightbmp.h"
#include "orangeblocktopleftrightbottombmp.h"
#include "orangeblocktoprightbmp.h"
#include "orangeblocktoprightbottombmp.h"

#include "blockexplode1bmp.h"
#include "blockexplode2bmp.h"
#include "blockexplode3bmp.h"
#include "blockexplode4bmp.h"
#include "blockexplode5bmp.h"

/**
 * An orange block.
 */
class OrangeBlock : public NormalBlock {
public:

	/**
	 * Constructor.
	 */
	OrangeBlock() : NormalBlock(woopsiRGB(31, 15, 0)) {
		_bitmaps[CONNECTION_NONE] = new OrangeBlockBmp();
		_bitmaps[CONNECTION_TOP] = new OrangeBlockTopBmp();
		_bitmaps[CONNECTION_BOTTOM] = new OrangeBlockBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new OrangeBlockTopBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new OrangeBlockTopLeftBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new OrangeBlockTopRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new OrangeBlockTopLeftRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new OrangeBlockTopLeftBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new OrangeBlockTopRightBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new OrangeBlockTopLeftRightBottomBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new OrangeBlockBottomLeftBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new OrangeBlockBottomRightBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new OrangeBlockBottomLeftRightBmp();
		_bitmaps[CONNECTION_LEFT] = new OrangeBlockLeftBmp();
		_bitmaps[CONNECTION_RIGHT] = new OrangeBlockRightBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new OrangeBlockLeftRightBmp();

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
	~OrangeBlock() { };

private:
	OrangeBlockExplodeBmp _explodeBmp1;	/**< First explosion bitmap. */
	BlockExplode1Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	BlockExplode2Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	BlockExplode3Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	BlockExplode4Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	BlockExplode5Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */

	OrangeBlockBounce1Bmp _bounceBmp1;	/**< First block bounce bitmap. */
	OrangeBlockBounce2Bmp _bounceBmp2;	/**< Second block bounce bitmap. */
	OrangeBlockBmp _bounceBmp3;			/**< Third block bounce bitmap. */
};

#endif
