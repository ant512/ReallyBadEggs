#ifndef _BLOCK_BASE_H_
#define _BLOCK_BASE_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

class BlockBase {
public:
	BlockBase(u16 colour) {
		_colour = colour;
		_topConnection = false;
		_leftConnection = false;
		_rightConnection = false;
		_bottomConnection = false;
	};

	virtual ~BlockBase() { };

	inline u16 getColour() const { return _colour; };

	inline bool hasLeftConnection() const { return _leftConnection; };
	inline bool hasRightConnection() const { return _rightConnection; };
	inline bool hasTopConnection() const { return _topConnection; };
	inline bool hasBottomConnection() const { return _bottomConnection; };

	virtual void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) = 0;

	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	};

protected:
	bool _topConnection;
	bool _leftConnection;
	bool _rightConnection;
	bool _bottomConnection;
	u16 _colour;

	WoopsiGfx::BitmapBase* _bitmap;
	WoopsiGfx::Animation* _landingAnim;
	WoopsiGfx::Animation* _explodingAnim;
};

#endif
