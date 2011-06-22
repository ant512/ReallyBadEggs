#ifndef _GREEN_BLOCK_H_
#define _GREEN_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

//#include "greenblockbmp.h"
//#include "greenblockbottombmp.h"
//#include "greenblockbottomleftbmp.h"
//#include "greenblockbottomleftrightbmp.h"
//#include "greenblockbottomrightbmp.h"
//#include "greenblockbounce1bmp.h"
//#include "greenblockbounce2bmp.h"
//#include "greenblockexplodebmp.h"
//#include "greenblockleftbmp.h"
//#include "greenblockleftrightbmp.h"
//#include "greenblockrightbmp.h"
//#include "greenblocktopbmp.h"
//#include "greenblocktopbottombmp.h"
//#include "greenblocktopleftbmp.h"
//#include "greenblocktopleftbottombmp.h"
//#include "greenblocktopleftrightbmp.h"
//#include "greenblocktopleftrightbottombmp.h"
//#include "greenblocktoprightbmp.h"
//#include "greenblocktoprightbottombmp.h"

#include "blockexplode1bmp.h"
#include "blockexplode2bmp.h"
#include "blockexplode3bmp.h"
#include "blockexplode4bmp.h"
#include "blockexplode5bmp.h"

/**
 * A green block.
 */
class GreenBlock : public NormalBlock {
public:

	/**
	 * Constructor.
	 */
	GreenBlock() : NormalBlock(woopsiRGB(0, 31, 0)) {
		//_bitmaps[CONNECTION_NONE] = new GreenBlockBmp();
		//_bitmaps[CONNECTION_TOP] = new GreenBlockTopBmp();
		//_bitmaps[CONNECTION_BOTTOM] = new GreenBlockBottomBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM] = new GreenBlockTopBottomBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_LEFT] = new GreenBlockTopLeftBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_RIGHT] = new GreenBlockTopRightBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_LEFT | CONNECTION_RIGHT] = new GreenBlockTopLeftRightBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT] = new GreenBlockTopLeftBottomBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_RIGHT] = new GreenBlockTopRightBottomBmp();
		//_bitmaps[CONNECTION_TOP | CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new GreenBlockTopLeftRightBottomBmp();
		//_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new GreenBlockBottomLeftBmp();
		//_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new GreenBlockBottomRightBmp();
		//_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new GreenBlockBottomLeftRightBmp();
		//_bitmaps[CONNECTION_LEFT] = new GreenBlockLeftBmp();
		//_bitmaps[CONNECTION_RIGHT] = new GreenBlockRightBmp();
		//_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new GreenBlockLeftRightBmp();

		//_explodingAnim->addFrame(&_explodeBmp1, 0);
		//_explodingAnim->addFrame(&_explodeBmp2, 0);
		//_explodingAnim->addFrame(&_explodeBmp3, 0);
		//_explodingAnim->addFrame(&_explodeBmp4, 0);
		//_explodingAnim->addFrame(&_explodeBmp5, 0);
		//_explodingAnim->addFrame(&_explodeBmp6, 0);

		//_landingAnim->addFrame(&_bounceBmp1, 0);
		//_landingAnim->addFrame(&_bounceBmp2, 0);
		//_landingAnim->addFrame(&_bounceBmp1, 0);
		//_landingAnim->addFrame(&_bounceBmp3, 0);
		//_landingAnim->addFrame(&_bounceBmp1, 0);
	};

	/**
	 * Destructor.
	 */
	~GreenBlock() { };

private:
	//GreenBlockExplodeBmp _explodeBmp1;	/**< First explosion bitmap. */
	//BlockExplode1Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	//BlockExplode2Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	//BlockExplode3Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	//BlockExplode4Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	//BlockExplode5Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */

	//GreenBlockBounce1Bmp _bounceBmp1;	/**< First block bounce bitmap. */
	//GreenBlockBounce2Bmp _bounceBmp2;	/**< Second block bounce bitmap. */
	//GreenBlockBmp _bounceBmp3;			/**< Third block bounce bitmap. */
};

#endif
