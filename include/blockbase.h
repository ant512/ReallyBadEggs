#ifndef _BLOCK_BASE_H_
#define _BLOCK_BASE_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

/**
 * Abstract base class for all blocks that appear in the grid.
 */
class BlockBase {
public:

	/**
	 * Constructor.
	 * @param colour The colour of the block.  This should be unique to the
	 * inheriting class so that blocks can be compared without resorting to
	 * RTTI.
	 */
	BlockBase(u16 colour);

	/**
	 * Destructor.
	 */
	virtual ~BlockBase();

	/**
	 * Gets the colour of the block. 
	 * @return The colour of the block.
	 */
	u16 getColour() const;

	/**
	 * Check if the block is connected to the block on its left.
	 * @return True if a connection exists; false if not.
	 */
	bool hasLeftConnection() const;

	/**
	 * Check if the block is connected to the block on its right.
	 * @return True if a connection exists; false if not.
	 */
	bool hasRightConnection() const;

	/**
	 * Check if the block is connected to the block above.
	 * @return True if a connection exists; false if not.
	 */
	bool hasTopConnection() const;

	/**
	 * Check if the block is connected to the block below.
	 * @return True if a connection exists; false if not.
	 */
	bool hasBottomConnection() const;

	/**
	 * Check if the block is currently landing.
	 * @return True if the block is landing.
	 */
	bool isLanding() const;

	/**
	 * Check if the block is currently falling.
	 * @return True if the block is falling.
	 */
	bool isFalling() const;

	/**
	 * Check if the block is currently exploding.
	 * @return True if the block is exploding.
	 */
	bool isExploding() const;

	/**
	 * Check if the block has finished exploding and needs to be removed from
	 * the grid.
	 * @return True if the block has exploded.
	 */
	bool isExploded() const;

	/**
	 * Check if the block can establish connections.
	 * @return True if the block can establish connections.
	 */
	bool isConnectable() const;

	/**
	 * Inform the block that it is falling.
	 */
	void fall();
	void explode();
	void land();

	void animate();

	bool hasDroppedHalfBlock() const { return _hasDroppedHalfBlock; };

	void dropHalfBlock() { _hasDroppedHalfBlock = !_hasDroppedHalfBlock; };

	/**
	 * Attempt to establish which of the surrounding blocks are of the same type
	 * as this and remember those connections.
	 * @param top The block above this.
	 * @param bottom The block below this.
	 * @param right The block to the right of this.
	 * @param left The block to the left of this.
	 */
	virtual void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) = 0;

	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

	const WoopsiGfx::BitmapBase* getBitmap() const;

protected:

	/**
	 * Bitmask of possible connections.
	 */
	enum ConnectionDirection {
		CONNECTION_NONE = 0,
		CONNECTION_TOP = 1,
		CONNECTION_LEFT = 2,
		CONNECTION_RIGHT = 4,
		CONNECTION_BOTTOM = 8
	};

	u8 _connections;						/**< Bitmask of active connections. */
	u16 _colour;							/**< Colour of the block. */
	bool _isExploding;						/**< True if the block is exploding. */
	bool _isLanding;						/**< True if the block is landing. */
	bool _isFalling;						/**< True if the block is falling. */
	bool _hasDroppedHalfBlock;

	WoopsiGfx::BitmapBase** _bitmaps;		/**< Array of bitmaps showing all possible connections. */
	WoopsiGfx::Animation* _landingAnim;		/**< Animation that plays when the block is landing. */
	WoopsiGfx::Animation* _explodingAnim;	/**< Animation that plays when the block is exploding. */

	void setConnections(bool top, bool right, bool bottom, bool left);
};

#endif
