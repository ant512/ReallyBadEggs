#ifndef _BLOCK_BASE_H_
#define _BLOCK_BASE_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

class BlockBase {
public:
	BlockBase(u16 colour);

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

	virtual void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) = 0;

	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

	const WoopsiGfx::BitmapBase* getBitmap() const;

protected:

	enum ConnectionDirection {
		CONNECTION_NONE = 0,
		CONNECTION_TOP = 1,
		CONNECTION_LEFT = 2,
		CONNECTION_RIGHT = 4,
		CONNECTION_BOTTOM = 8
	};

	u8 _connections;
	u16 _colour;
	bool _isExploding;
	bool _isLanding;
	bool _isFalling;

	WoopsiGfx::BitmapBase** _bitmaps;
	WoopsiGfx::Animation* _landingAnim;
	WoopsiGfx::Animation* _explodingAnim;

	void setConnections(bool top, bool right, bool bottom, bool left);
};

#endif
