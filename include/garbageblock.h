#ifndef _GARBAGE_BLOCK_H_
#define _GARBAGE_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "blockbase.h"

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
};

#endif
