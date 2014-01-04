#include <dmafuncs.h>
#include <rect.h>

#include "framebuffer.h"

FrameBuffer::FrameBuffer(u16* data, u16 width, u16 height) {
	_width = width;
	_height = height;
	_bitmap = data;
	_backBuffer = NULL;
}

FrameBuffer::FrameBuffer(u16* data, u16* backBuffer, u16 width, u16 height) {
	_width = width;
	_height = height;
	_bitmap = data;
	_backBuffer = backBuffer;
}

// Get a single pixel from the bitmap
const u16 FrameBuffer::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap[pos];
}

// Draw a single pixel to the bitmap
void FrameBuffer::setPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Plot the pixel
	u32 pos = (y * _width) + x;
	_bitmap[pos] = colour;
}

const u16* FrameBuffer::getData(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap + pos;
}

void FrameBuffer::blit(const s16 x, const s16 y, const u16* data, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(data, pos, size);
}

void FrameBuffer::blitFill(const s16 x, const s16 y, const u16 colour, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaFill(colour, pos, size);
}

void FrameBuffer::copy(s16 x, s16 y, u32 size, u16* dest) const {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(pos, dest, size);
}

void FrameBuffer::flipBuffer() {

	if (_backBuffer == NULL) return;

	u16* tmp = _bitmap;
	_bitmap = _backBuffer;
	_backBuffer = tmp;
}

void FrameBuffer::buffer() {
	if (_backBuffer == NULL) return;
	
	copy(0, 0, _width * _height, _backBuffer);
}

WoopsiGfx::Graphics* FrameBuffer::newGraphics() {
	WoopsiGfx::Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = _width;
	rect.height = _height;
	return new WoopsiGfx::Graphics(this, rect);
}
