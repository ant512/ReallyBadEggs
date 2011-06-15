#ifndef _GREY_BLOCK_H_
#define _GREY_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "blockbase.h"

/**
 * Grey blocks cannot connect to any other block.  They are removed from the
 * grid when they are next to other blocks that explode.
 */
class GreyBlock : public BlockBase {
public:

	/**
	 * Constructor.
	 */
	GreyBlock();

	/**
	 * Destructor.
	 */
	~GreyBlock();

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
