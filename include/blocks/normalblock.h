#ifndef _NORMAL_BLOCK_H_
#define _NORMAL_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "blockbase.h"

/**
 * Standard block that will connect to others of the same colour.
 */
class NormalBlock : public BlockBase {
public:

	/**
	 * Constructor.
	 * @param colour Colour of the block.  This should at least represent the
	 * colour of the block's bitmaps.  It should be unique so that this block
	 * can identify others of the same type (RTTI is not used).
	 */
	NormalBlock(u16 colour);

	/**
	 * Destructor.
	 */
	virtual ~NormalBlock();

	/**
	 * Attempt to establish which of the surrounding blocks are of the same type
	 * as this and remember those connections.
	 * @param top The block above this.
	 * @param bottom The block below this.
	 * @param right The block to the right of this.
	 * @param left The block to the left of this.
	 */
	void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left);
};

#endif
