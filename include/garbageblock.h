#ifndef _GARBAGE_BLOCK_H_
#define _GARBAGE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "blockbase.h"

#include "garbageblockexplode1bmp.h"
#include "garbageblockexplode2bmp.h"
#include "garbageblockexplode3bmp.h"
#include "garbageblockexplode4bmp.h"
#include "garbageblockexplode5bmp.h"
#include "garbageblockexplode6bmp.h"

/**
 * Garbage blocks cannot connect to any other block.  They are removed from the
 * grid when they are next to other blocks that explode.
 */
class GarbageBlock : public BlockBase {
public:

	static const u16 COLOUR = 42281;	/**< Block colour; woopsiRGB(9, 9, 9) */

	/**
	 * Constructor.
	 */
	GarbageBlock();

	/**
	 * Destructor.
	 */
	~GarbageBlock();

	/**
	 * Will not attempt to create any connections.
	 * @param top The block above this.
	 * @param bottom The block below this.
	 * @param right The block to the right of this.
	 * @param left The block to the left of this.
	 */
	void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) { };

private:
	GarbageBlockExplode1Bmp _explodeBmp1;		/**< First explosion bitmap. */
	GarbageBlockExplode2Bmp _explodeBmp2;		/**< Second explosion bitmap. */
	GarbageBlockExplode3Bmp _explodeBmp3;		/**< Third explosion bitmap. */
	GarbageBlockExplode4Bmp _explodeBmp4;		/**< Fourth explosion bitmap. */
	GarbageBlockExplode5Bmp _explodeBmp5;		/**< Fifth explosion bitmap. */
	GarbageBlockExplode6Bmp _explodeBmp6;		/**< Sixth explosion bitmap. */
};

#endif
