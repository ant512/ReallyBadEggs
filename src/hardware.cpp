#include "hardware.h"

PadState Hardware::_pad;
StylusState Hardware::_stylus;

SDLFrameBuffer* Hardware::_topBuffer = NULL;
SDLFrameBuffer* Hardware::_bottomBuffer = NULL;

WoopsiGfx::Graphics* Hardware::_topGfx = NULL;
WoopsiGfx::Graphics* Hardware::_bottomGfx = NULL;

bool Hardware::_isMostRecentDirectionVertical = false;

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
	
	_pad.up = false;
	_pad.down = false;
	_pad.left = false;
	_pad.right = false;
	_pad.l = false;
	_pad.r = false;
	_pad.a = false;
	_pad.b = false;
	_pad.x = false;
	_pad.y = false;
	_pad.start = false;
	_pad.select = false;

	updatePadState();
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

void Hardware::updatePadState() {

	PadState oldPad = _pad;

#ifndef USING_SDL

	scanKeys();

	s32 pressed = keysDown();	// buttons pressed this loop
	s32 held = keysHeld();		// buttons currently held
	s32 allKeys = pressed | held;

	_pad.up = allKeys & KEY_UP;
	_pad.down = allKeys & KEY_DOWN;
	_pad.left = allKeys & KEY_LEFT;
	_pad.right = allKeys & KEY_RIGHT;
	_pad.l = allKeys & KEY_L;
	_pad.r = allKeys & KEY_R;
	_pad.a = allKeys & KEY_A;
	_pad.b = allKeys & KEY_B;
	_pad.x = allKeys & KEY_X;
	_pad.y = allKeys & KEY_Y;
	_pad.start = allKeys & KEY_START;
	_pad.select = allKeys & KEY_SELECT;

#else

	Uint8* keyState = SDL_GetKeyState(NULL);

	_pad.up = keyState[SDLK_UP];
	_pad.down = keyState[SDLK_DOWN];
	_pad.left = keyState[SDLK_LEFT];
	_pad.right = keyState[SDLK_RIGHT];
	_pad.l = keyState[SDLK_a];
	_pad.r = keyState[SDLK_s];
	_pad.a = keyState[SDLK_z];
	_pad.b = keyState[SDLK_x];
	_pad.x = keyState[SDLK_c];
	_pad.y = keyState[SDLK_v];
	_pad.start = keyState[SDLK_d];
	_pad.select = keyState[SDLK_f];

#endif

	if ((_pad.up && !oldPad.up) || (_pad.down && !oldPad.down)) {
		_isMostRecentDirectionVertical = true;
	} else if ((_pad.left && !oldPad.left) || (_pad.right && !oldPad.right)) {
		_isMostRecentDirectionVertical = false;
	}

	if (_stylus.newPress) {
		_stylus.held = true;
		_stylus.newPress = false;
	}

	_stylus.released = false;

#ifndef USING_SDL

	if ((allKeys & KEY_TOUCH) && (!_stylus.held)) {
		
		// New click
		_stylus.newPress = true;
		_stylus.released = false;

	} else if ((!(allKeys & KEY_TOUCH)) && (_stylus.held)) {
		
		// Release
		_stylus.released = true;
		_stylus.newPress = false;
		_stylus.held = false;
	}

	touchPosition touch;
	touchRead(&touch);

	_stylus.x = touch.px;
	_stylus.y = touch.py;

#else

	// Read mouse state
	int mouseX;
	int mouseY;
	
	int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	
	// Check buttons
	if ((mouseState & SDL_BUTTON_LEFT) && (!_stylus.held)) {
		
		// New click
		_stylus.newPress = true;
		_stylus.released = false;

	} else if ((!(mouseState & SDL_BUTTON_LEFT)) && (_stylus.held)) {
		
		// Release
		_stylus.released = true;
		_stylus.newPress = false;
		_stylus.held = false;
	}
	
	_stylus.x = mouseX;
	_stylus.y = mouseY - SCREEN_HEIGHT;
	
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
}

bool Hardware::isMostRecentDirectionVertical() {
	return _isMostRecentDirectionVertical;
}

void Hardware::waitForVBlank() {

#ifndef USING_SDL

	swiWaitForVBlank();

#else

	SDL_Delay(10);
	SDL_Flip(_surface);

#endif

	updatePadState();
}
