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
		_connections = 0;
		_isFalling = true;
		_isLanding = false;
		_isExploding = false;

		_bitmaps = new WoopsiGfx::BitmapBase*[16];

		for (s32 i = 0; i < 16; ++i) {
			_bitmaps[i] = NULL;
		}
	};

	virtual ~BlockBase() {
		for (s32 i = 0; i < 16; ++i) {
			if (_bitmaps[i] != NULL) delete _bitmaps[i];
		}

		delete[] _bitmaps;
	};

	inline u16 getColour() const { return _colour; };

	inline bool hasLeftConnection() const { return _connections & CONNECTION_LEFT; };
	inline bool hasRightConnection() const { return _connections & CONNECTION_RIGHT; };
	inline bool hasTopConnection() const { return _connections & CONNECTION_TOP; };
	inline bool hasBottomConnection() const { return _connections & CONNECTION_BOTTOM; };

	inline bool isLanding() const { return _isLanding; };
	inline bool isFalling() const { return _isFalling; };
	inline bool isExploding() const { return _isExploding; };
	inline bool isConnectable() const { return !_isLanding && !_isFalling && !_isExploding; };

	inline void setLanded(bool landed) { _isLanding = landed; };
	inline void setFalling(bool falling) { _isFalling = falling; };
	inline void setExploding(bool exploding) { _isExploding = exploding; };

	virtual void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) = 0;

	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {
		const WoopsiGfx::BitmapBase* bmp = getBitmap();

		gfx->drawBitmap(x, y, bmp->getWidth(), bmp->getHeight(), bmp, 0, 0);
	};

	const WoopsiGfx::BitmapBase* getBitmap() const {
		if (_isExploding) {
			return _explodingAnim->getCurrentBitmap();
		} else if (_isLanding)  {
			return _landingAnim->getCurrentBitmap();
		}

		return _bitmaps[_connections];
	};

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

	void setConnections(bool top, bool right, bool bottom, bool left) {
		_connections = top | (left << 1) | (right << 2) | (bottom << 3);
	};
};

#endif
