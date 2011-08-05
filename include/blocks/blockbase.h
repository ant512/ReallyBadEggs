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
	 * List of all possible block states.
	 */
	enum BlockState {
		BLOCK_STATE_NORMAL = 0,		/**< Block is doing nothing. */
		BLOCK_STATE_FALLING = 1,	/**< Block is falling down the grid. */
		BLOCK_STATE_LANDING = 2,	/**< Block is landing. */
		BLOCK_STATE_EXPLODING = 3,	/**< Block is exploding. */
		BLOCK_STATE_EXPLODED = 4	/**< Block has exploded. */
	};

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
	 * Get the state of the block.
	 * @return The state of the block.
	 */
	BlockState getState() const;

	/**
	 * Inform the block that it is falling.
	 */
	void fall();

	/**
	 * Explode the block.  Starts the explosion animation.
	 */
	void explode();

	/**
	 * Inform the block that it has landed.
	 */
	void land();

	/**
	 * If any animations - landing or exploding - are active, the animations
	 * run.  If the block is landing and the landing animation has finished, the
	 * isLanding() property is set to false.  Alternatively, if the block is
	 * exploding and the explosion animation has finished, the isExploded()
	 * property is set to true.
	 */
	void animate();

	/**
	 * Check if the block has dropped a half block.  The grid is designed so
	 * that each item in its array represents a whole block, but blocks can drop
	 * half-way.  To represent this, the blocks include a bit that indicates
	 * that they have dropped half-way through a grid square.  They only
	 * actually move when they are examined and the bit is already set.  The bit
	 * is flipped each time an attempt is made to drop the block.
	 * @return True if the block has dropped half a grid square.
	 */
	bool hasDroppedHalfBlock() const { return _hasDroppedHalfBlock; };

	/**
	 * Inform the block that it has dropped half a grid square.
	 * @return True if the block has dropped half a grid square.
	 */
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

	/**
	 * Draws the block to the supplied graphics object at the specified
	 * co-ordinates.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 * @param gfx The graphics object to draw to.
	 */
	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

protected:

	/**
	 * Bitmask of possible connections.
	 */
	enum ConnectionDirection {
		CONNECTION_NONE = 0,				/**< No connections. */
		CONNECTION_TOP = 1,					/**< Top connection. */
		CONNECTION_LEFT = 2,				/**< Left connection. */
		CONNECTION_RIGHT = 4,				/**< Right connection. */
		CONNECTION_BOTTOM = 8				/**< Bottom connection. */
	};

	u8 _connections;						/**< Bitmask of active connections. */
	u16 _colour;							/**< Colour of the block. */
	BlockState _state;
	bool _hasDroppedHalfBlock;				/**< True if the block has dropped half a grid square. */

	WoopsiGfx::BitmapBase** _bitmaps;		/**< Array of bitmaps showing all possible connections. */
	WoopsiGfx::Animation* _landingAnim;		/**< Animation that plays when the block is landing. */
	WoopsiGfx::Animation* _explodingAnim;	/**< Animation that plays when the block is exploding. */

	/**
	 * Sets the connections that the block has to the supplied parameters.
	 * @param top The state of the top connection.
	 * @param right The state of the right connection.
	 * @param bottom The state of the bottom connection.
	 * @param left The state of the left connection.
	 */
	void setConnections(bool top, bool right, bool bottom, bool left);

	/**
	 * Gets the current bitmap based on the block's state.
	 * @return The current bitmap.
	 */
	const WoopsiGfx::BitmapBase* getBitmap() const;
};

#endif
