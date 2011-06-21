#ifndef _PURPLE_BLOCK_H_
#define _PURPLE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "purpleblockbmp.h"
#include "purpleblockbottombmp.h"
//#include "purpleblockbottomleftbmp.h"
//#include "purpleblockbottomleftrightbmp.h"
//#include "purpleblockbottomrightbmp.h"
#include "purpleblockbounce1bmp.h"
#include "purpleblockbounce2bmp.h"
#include "purpleblockexplodebmp.h"
#include "purpleblockleftbmp.h"
//#include "purpleblockleftrightbmp.h"
#include "purpleblockrightbmp.h"
#include "purpleblocktopbmp.h"
#include "purpleblocktopbottombmp.h"
#include "purpleblocktopleftbmp.h"
//#include "purpleblocktopleftbottombmp.h"
//#include "purpleblocktopleftrightbmp.h"
//#include "purpleblocktopleftrightbottombmp.h"
#include "purpleblocktoprightbmp.h"
//#include "purpleblocktoprightbottombmp.h"

#include "blockexplode1bmp.h"
#include "blockexplode2bmp.h"
#include "blockexplode3bmp.h"
#include "blockexplode4bmp.h"
#include "blockexplode5bmp.h"

/**
 * A purple block.
 */
class PurpleBlock : public NormalBlock {
public:

	/**
	 * Constructor.
	 */
	PurpleBlock() : NormalBlock(woopsiRGB(31, 0, 31)) {
		_bitmaps[CONNECTION_NONE] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP] = new PurpleBlockTopBmp();
		_bitmaps[CONNECTION_BOTTOM] = new PurpleBlockBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new PurpleBlockTopBottomBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new PurpleBlockTopLeftBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new PurpleBlockTopRightBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new PurpleBlockLeftBmp();
		_bitmaps[CONNECTION_RIGHT] = new PurpleBlockRightBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new PurpleBlockBmp();

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
	~PurpleBlock() { };

private:
	PurpleBlockExplodeBmp _explodeBmp1;	/**< First explosion bitmap. */
	BlockExplode1Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	BlockExplode2Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	BlockExplode3Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	BlockExplode4Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	BlockExplode5Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */

	PurpleBlockBounce1Bmp _bounceBmp1;	/**< First block bounce bitmap. */
	PurpleBlockBounce2Bmp _bounceBmp2;	/**< Second block bounce bitmap. */
	PurpleBlockBmp _bounceBmp3;			/**< Third block bounce bitmap. */
};

#endif
