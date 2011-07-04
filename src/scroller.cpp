#include "scroller.h"

Scroller::Scroller(const WoopsiGfx::WoopsiString& text) {
	_offset = -SCREEN_WIDTH;
	_text = text;

	_stringWidth = _font.getStringWidth(text);
}

void Scroller::render(s32 y, WoopsiGfx::Graphics* gfx) {
	s32 start = _offset / 8;
	s32 length = (SCREEN_WIDTH / 8) + 8;
	s32 x = -(_offset % 8);

	if (start < 0) {
		x -= start * 8;
		start = 0;
	}

	gfx->drawFilledRect(0, y, SCREEN_WIDTH, _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, _text, start, length, woopsiRGB(31, 31, 31));

	_offset += 2;

	if (_offset - _stringWidth > 0) _offset = -SCREEN_WIDTH;
}
