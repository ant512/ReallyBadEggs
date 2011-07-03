#include "blockbase.h"

BlockBase::BlockBase(u16 colour) {
	_colour = colour;
	_connections = 0;
	_isFalling = true;
	_isLanding = false;
	_isExploding = false;
	_hasDroppedHalfBlock = false;

	_bitmaps = new WoopsiGfx::BitmapBase*[16];

	for (s32 i = 0; i < 16; ++i) {
		_bitmaps[i] = NULL;
	}

	_landingAnim = new WoopsiGfx::Animation(1, WoopsiGfx::Animation::ANIMATION_LOOPTYPE_NONE, 0);
	_explodingAnim = new WoopsiGfx::Animation(1, WoopsiGfx::Animation::ANIMATION_LOOPTYPE_NONE, 0);
}

BlockBase::~BlockBase() {
	for (s32 i = 0; i < 16; ++i) {
		if (_bitmaps[i] != NULL) delete _bitmaps[i];
	}

	delete[] _bitmaps;

	delete _landingAnim;
	delete _explodingAnim;
}

u16 BlockBase::getColour() const {
	return _colour;
}

bool BlockBase::hasLeftConnection() const {
	return _connections & CONNECTION_LEFT;
}

bool BlockBase::hasRightConnection() const {
	return _connections & CONNECTION_RIGHT;
}

bool BlockBase::hasTopConnection() const {
	return _connections & CONNECTION_TOP;
}

bool BlockBase::hasBottomConnection() const {
	return _connections & CONNECTION_BOTTOM;
}

bool BlockBase::isLanding() const {
	return _isLanding;
}

bool BlockBase::isFalling() const {
	return _isFalling;
}

bool BlockBase::isExploding() const {
	return _isExploding;
}

bool BlockBase::isExploded() const {
	return _isExploding && _explodingAnim->getStatus() == WoopsiGfx::Animation::ANIMATION_STATUS_STOPPED;
};

bool BlockBase::isConnectable() const {
	return !_isLanding && !_isFalling && !_isExploding;
}

void BlockBase::fall() {
	_isFalling = true;
	_isLanding = false;
}

void BlockBase::land() {
	_isFalling = false;
	_isLanding = true;
	_landingAnim->play();
}

void BlockBase::explode() {
	_isExploding = true;
	_explodingAnim->play();
}

void BlockBase::animate() {
	if (_isExploding) {
		_explodingAnim->run();
	} else if (_isLanding) {
		_landingAnim->run();

		if (_landingAnim->getStatus() == WoopsiGfx::Animation::ANIMATION_STATUS_STOPPED) {
			_isLanding = false;
		}
	}
}

void BlockBase::render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {
	const WoopsiGfx::BitmapBase* bmp = getBitmap();

	gfx->drawBitmap(x, y, bmp->getWidth(), bmp->getHeight(), bmp, 0, 0);
	//gfx->drawFilledRect(x, y, 3, 3, getColour());
}

const WoopsiGfx::BitmapBase* BlockBase::getBitmap() const {
	if (_isExploding) {
		return _explodingAnim->getCurrentBitmap();
	} else if (_isLanding)  {
		return _landingAnim->getCurrentBitmap();
	}

	return _bitmaps[_connections];
}

void BlockBase::setConnections(bool top, bool right, bool bottom, bool left) {
	_connections = top | (left << 1) | (right << 2) | (bottom << 3);
}
