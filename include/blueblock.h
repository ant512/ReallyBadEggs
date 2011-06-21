#ifndef _BLUE_BLOCK_H_
#define _BLUE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "normalblock.h"

#include "blueblockbmp.h"
#include "blueblockbottombmp.h"
#include "blueblockbottomleftbmp.h"
//#include "blueblockbottomleftrightbmp.h"
#include "blueblockbottomrightbmp.h"
#include "blueblockbounce1bmp.h"
#include "blueblockbounce2bmp.h"
#include "blueblockexplodebmp.h"
#include "blueblockleftbmp.h"
#include "blueblockleftrightbmp.h"
#include "blueblockrightbmp.h"
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

/**
 * A blue block.
 */
class BlueBlock : public NormalBlock {
public:

	/**
	 * Constructor.
	 */
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
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT] = new BlueBlockBottomLeftBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_RIGHT] = new BlueBlockBottomRightBmp();
		_bitmaps[CONNECTION_BOTTOM | CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockBmp();
		_bitmaps[CONNECTION_LEFT] = new BlueBlockLeftBmp();
		_bitmaps[CONNECTION_RIGHT] = new BlueBlockRightBmp();
		_bitmaps[CONNECTION_LEFT | CONNECTION_RIGHT] = new BlueBlockLeftRightBmp();

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
	~BlueBlock() { };

private:
	BlueBlockExplodeBmp _explodeBmp1;	/**< First explosion bitmap. */
	BlockExplode1Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	BlockExplode2Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	BlockExplode3Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	BlockExplode4Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	BlockExplode5Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */

	BlueBlockBounce1Bmp _bounceBmp1;	/**< First block bounce bitmap. */
	BlueBlockBounce2Bmp _bounceBmp2;	/**< Second block bounce bitmap. */
	BlueBlockBmp _bounceBmp3;			/**< Third block bounce bitmap. */
};

#endif
