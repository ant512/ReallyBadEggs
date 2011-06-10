#include "hardware.h"

Pad Hardware::_pad;
Stylus Hardware::_stylus;

SDLFrameBuffer* Hardware::_topBuffer = NULL;
SDLFrameBuffer* Hardware::_bottomBuffer = NULL;

WoopsiGfx::Graphics* Hardware::_topGfx = NULL;
WoopsiGfx::Graphics* Hardware::_bottomGfx = NULL;

#ifdef USING_SDL
SDL_Surface* Hardware::_surface = NULL;
#endif

void Hardware::init() {

#ifndef USING_SDL

	powerOn(POWER_ALL_2D);

	videoSetMode( MODE_5_2D | DISPLAY_BG3_ACTIVE );
	videoSetModeSub( MODE_5_2D | DISPLAY_BG3_ACTIVE );

	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankC( VRAM_C_SUB_BG );

	// Initialise backgrounds
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	_topBuffer = new SDLFrameBuffer((u16*)BG_BMP_RAM(0), SCREEN_WIDTH, SCREEN_HEIGHT);
	_bottomBuffer = new SDLFrameBuffer((u16*)BG_BMP_RAM_SUB(0), SCREEN_WIDTH, SCREEN_HEIGHT);

#else

	Uint32 initflags = SDL_INIT_VIDEO;
	Uint8 video_bpp = 0;
	Uint32 videoflags = SDL_SWSURFACE;

	// Initialize the SDL library
	if (SDL_Init(initflags) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Set video mode
	_surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT * 2, video_bpp, videoflags);
	if (_surface == NULL) {
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	_topBuffer = new SDLFrameBuffer(_surface, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	_bottomBuffer = new SDLFrameBuffer(_surface, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HEIGHT);

#endif

	_topGfx = _topBuffer->newGraphics();
	_bottomGfx = _bottomBuffer->newGraphics();
}

void Hardware::shutdown() {
	delete _topGfx;
	delete _bottomGfx;
	delete _topBuffer;
	delete _bottomBuffer;
	
#ifdef USING_SDL
	delete _surface;
#endif
}

void Hardware::waitForVBlank() {

#ifndef USING_SDL

	swiWaitForVBlank();
	scanKeys();

#else

	SDL_Delay(10);
	SDL_Flip(_surface);

	// SDL event pump
	SDL_Event event;
	
	// Check for SDL quit
	while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
				return;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == 53) {
                    // Escape pressed
					exit(0);
					return;
                }
                break;
        }
	}

#endif

	_pad.update();
	_stylus.update();
}
