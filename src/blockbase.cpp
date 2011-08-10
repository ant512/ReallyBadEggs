#include "blockbase.h"

BlockBase::BlockBase(u16 colour) {
	_colour = colour;
	_connections = 0;
	_state = BLOCK_STATE_NORMAL;
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

BlockBase::BlockState BlockBase::getState() const {
	return _state;
}

void BlockBase::fall() {
	_state = BLOCK_STATE_FALLING;
}

void BlockBase::land() {
	_state = BLOCK_STATE_LANDING;
	_landingAnim->play();
}

void BlockBase::explode() {
	_state = BLOCK_STATE_EXPLODING;
	_explodingAnim->play();
}

void BlockBase::animate() {
	switch (_state) {
		case BLOCK_STATE_EXPLODING:
			_explodingAnim->run();

			if (_explodingAnim->getStatus() == WoopsiGfx::Animation::ANIMATION_STATUS_STOPPED) {
				_state = BLOCK_STATE_EXPLODED;
			}
			break;
		
		case BLOCK_STATE_LANDING:
			_landingAnim->run();

			if (_landingAnim->getStatus() == WoopsiGfx::Animation::ANIMATION_STATUS_STOPPED) {
				_state = BLOCK_STATE_NORMAL;
			}
			break;
		
		default:
			break;
	}
}

void BlockBase::render(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {
	const WoopsiGfx::BitmapBase* bmp = getBitmap();

	gfx->drawBitmap(x, y, bmp->getWidth(), bmp->getHeight(), bmp, 0, 0);
}

const WoopsiGfx::BitmapBase* BlockBase::getBitmap() const {
	switch (_state) {
		case BLOCK_STATE_EXPLODING:
		case BLOCK_STATE_EXPLODED:
			return _explodingAnim->getCurrentBitmap();
		
		case BLOCK_STATE_LANDING:
			return _landingAnim->getCurrentBitmap();
	
		default:
			return _bitmaps[_connections];
	}
}

void BlockBase::setConnections(bool top, bool right, bool bottom, bool left) {
	_connections = top | (left << 1) | (right << 2) | (bottom << 3);
}
