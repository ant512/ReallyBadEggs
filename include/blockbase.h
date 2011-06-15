#ifndef _BLOCK_BASE_H_
#define _BLOCK_BASE_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

class BlockBase {
public:
	BlockBase(u16 colour);

	/**
	 * Destructor.
	 */
	virtual ~BlockBase();

	u16 getColour() const;

	bool hasLeftConnection() const;
	bool hasRightConnection() const;
	bool hasTopConnection() const;
	bool hasBottomConnection() const;

	bool isLanding() const;
	bool isFalling() const;
	bool isExploding() const;
	bool isExploded() const;

	bool isConnectable() const;

	void fall();
	void explode();
	void land();

	void animate();

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

	WoopsiGfx::BitmapBase** _bitmaps;		/**< Array of bitmaps showing all possible connections. */
	WoopsiGfx::Animation* _landingAnim;		/**< Animation that plays when the block is landing. */
	WoopsiGfx::Animation* _explodingAnim;	/**< Animation that plays when the block is exploding. */

	void setConnections(bool top, bool right, bool bottom, bool left);
};

#endif
