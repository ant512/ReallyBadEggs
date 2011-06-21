#ifndef _YELLOW_BLOCK_H_
#define _YELLOW_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "yellowblockbmp.h"
#include "yellowblockbottombmp.h"
//#include "yellowblockbottomleftbmp.h"
	//#include "yellowblockbottomleftrightbmp.h"
	//#include "yellowblockbottomrightbmp.h"
#include "yellowblockbounce1bmp.h"
#include "yellowblockbounce2bmp.h"
#include "yellowblockexplodebmp.h"
#include "yellowblockleftbmp.h"
//#include "yellowblockleftrightbmp.h"
#include "yellowblockrightbmp.h"
#include "yellowblocktopbmp.h"
#include "yellowblocktopbottombmp.h"
#include "yellowblocktopleftbmp.h"
//#include "yellowblocktopleftbottombmp.h"
//#include "yellowblocktopleftrightbmp.h"
//#include "yellowblocktopleftrightbottombmp.h"
#include "yellowblocktoprightbmp.h"
	//#include "yellowblocktoprightbottombmp.h"

#include "blockexplode1bmp.h"
#include "blockexplode2bmp.h"
#include "blockexplode3bmp.h"
#include "blockexplode4bmp.h"
#include "blockexplode5bmp.h"

/**
 * A yellow block.
 */
class YellowBlock : public NormalBlock {
public:

	/**
	 * Constructor.
	 */
	YellowBlock() : NormalBlock(woopsiRGB(31, 31, 0)) {
		_bitmaps[CONNECTION_NONE] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM] = new YellowBlockBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new YellowBlockTopBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new YellowBlockTopLeftBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new YellowBlockTopRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new YellowBlockLeftBmp();
		_bitmaps[CONNECTION_RIGHT] = new YellowBlockRightBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new YellowBlockBmp();

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
	~YellowBlock() { };

private:
	YellowBlockExplodeBmp _explodeBmp1;	/**< First explosion bitmap. */
	BlockExplode1Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	BlockExplode2Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	BlockExplode3Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	BlockExplode4Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	BlockExplode5Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */

	YellowBlockBounce1Bmp _bounceBmp1;	/**< First block bounce bitmap. */
	YellowBlockBounce2Bmp _bounceBmp2;	/**< Second block bounce bitmap. */
	YellowBlockBmp _bounceBmp3;			/**< Third block bounce bitmap. */
};

#endif
