#include <dmafuncs.h>
#include <rect.h>

#include "sdlframebuffer.h"

#ifdef USING_SDL

// SDL mode

SDLFrameBuffer::SDLFrameBuffer(SDL_Surface* surface, u16 width, u16 height, u16 yOffset) {
	_width = width;
	_height = height;
	_surface = surface;
	_yOffset = yOffset;
	_dataBuffer = new u16[width * height];
}

const u16 SDLFrameBuffer::getPixel(s16 x, s16 y) const {
	
	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;
	
	return _dataBuffer[x + (y * _width)];
}

void SDLFrameBuffer::setPixel(s16 x, s16 y, u16 colour) {
	
	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;
	
	// Get the pixel colour in 24-bit format
	u32 r = (colour & 31) << 3;
	u32 g = (colour & (31 << 5)) >> 2;
	u32 b = (colour & (31 << 10)) >> 7;
	
	// Plot the pixel
	SDL_PixelFormat* format = _surface->format;
	SDL_LockSurface(_surface);
	
	putSDLPixel(x, y + _yOffset, SDL_MapRGB(format, r, g, b));
	
	SDL_UnlockSurface(_surface);

	// Plot the pixel in the buffer
	_dataBuffer[x + (y * _width)] = colour;
}

const u16* SDLFrameBuffer::getData() const {
	return getData(0, 0);
}

const u16* SDLFrameBuffer::getData(s16 x, s16 y) const {
	return _dataBuffer + (x + (y * _width));
}

void SDLFrameBuffer::copy(s16 x, s16 y, u32 size, u16* dest) const {
	u16* pos = _dataBuffer + (x + (y * _width));
	woopsiDmaCopy(pos, dest, size);
}

void SDLFrameBuffer::blit(const s16 x, const s16 y, const u16* data, const u32 size) {
	u16 destX = x;
	u16 destY = y + _yOffset;
	
	u32 r;
	u32 g;
	u32 b;
	
	SDL_PixelFormat* format = _surface->format;
	SDL_LockSurface(_surface);
	
	for (u32 i = 0; i < size; ++i) {
		
		// Get the pixel colour in 24-bit format
		r = (data[i] & 31) << 3;
		g = (data[i] & (31 << 5)) >> 2;
		b = (data[i] & (31 << 10)) >> 7;
		
		// Plot the pixel
		putSDLPixel(destX, destY, SDL_MapRGB(format, r, g, b));
		
		destX++;
		
		if (destX % _width == 0) {
			destX = 0;
			destY++;
		}
	}
	
	SDL_UnlockSurface(_surface);
	
	// Update the buffer
	u16* pos = _dataBuffer + (y * _width) + x;
	woopsiDmaCopy(data, pos, size);
}

void SDLFrameBuffer::blitFill(const s16 x, const s16 y, const u16 colour, const u32 size) {
	u16 destX = x;
	u16 destY = y + _yOffset;
	
	u32 r;
	u32 g;
	u32 b;
	
	SDL_PixelFormat* format = _surface->format;
	SDL_LockSurface(_surface);
	
	for (u32 i = 0; i < size; ++i) {
		
		// Get the pixel colour in 24-bit format
		r = (colour & 31) << 3;
		g = (colour & (31 << 5)) >> 2;
		b = (colour & (31 << 10)) >> 7;
		
		// Plot the pixel
		putSDLPixel(destX, destY, SDL_MapRGB(format, r, g, b));
		
		destX++;
		
		if (destX % _width == 0) {
			destX = 0;
			destY++;
		}
	}
	
	SDL_UnlockSurface(_surface);
	
	// Update the buffer
	u16* pos = _dataBuffer + (y * _width) + x;
	woopsiDmaFill(colour, pos, size);
}

void SDLFrameBuffer::putSDLPixel(int x, int y, Uint32 pixel) {
	int bpp = _surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)_surface->pixels + y * _surface->pitch + x * bpp;

	switch (bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

Uint32 SDLFrameBuffer::getSDLPixel(int x, int y) {
	int bpp = _surface->format->BytesPerPixel;
	
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)_surface->pixels + y * _surface->pitch + x * bpp;
	
	switch (bpp) {
		case 1:
			return *p;
			
		case 2:
			return *(Uint16 *)p;
			
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			
		case 4:
			return *(Uint32 *)p;
			
		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void SDLFrameBuffer::buffer() { }

#else

// DS mode

SDLFrameBuffer::SDLFrameBuffer(u16* data, u16 width, u16 height) {
	_width = width;
	_height = height;
	_bitmap = data;
	_backBuffer = NULL;
}

SDLFrameBuffer::SDLFrameBuffer(u16* data, u16* backBuffer, u16 width, u16 height) {
	_width = width;
	_height = height;
	_bitmap = data;
	_backBuffer = backBuffer;
}

// Get a single pixel from the bitmap
const u16 SDLFrameBuffer::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap[pos];
}

// Draw a single pixel to the bitmap
void SDLFrameBuffer::setPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Plot the pixel
	u32 pos = (y * _width) + x;
	_bitmap[pos] = colour;
}

const u16* SDLFrameBuffer::getData(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap + pos;
}

void SDLFrameBuffer::blit(const s16 x, const s16 y, const u16* data, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(data, pos, size);
}

void SDLFrameBuffer::blitFill(const s16 x, const s16 y, const u16 colour, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaFill(colour, pos, size);
}

void SDLFrameBuffer::copy(s16 x, s16 y, u32 size, u16* dest) const {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(pos, dest, size);
}

void SDLFrameBuffer::flipBuffer() {

	if (_backBuffer == NULL) return;

	u16* tmp = _bitmap;
	_bitmap = _backBuffer;
	_backBuffer = tmp;
}

void SDLFrameBuffer::buffer() {
	if (_backBuffer == NULL) return;
	
	copy(0, 0, _width * _height, _backBuffer);
}

#endif

WoopsiGfx::Graphics* SDLFrameBuffer::newGraphics() {
	WoopsiGfx::Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = _width;
	rect.height = _height;
	return new WoopsiGfx::Graphics(this, rect);
}
