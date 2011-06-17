#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <nds.h>
#include <graphics.h>

#include "sdlframebuffer.h"
#include "pad.h"
#include "stylus.h"

/**
 * Provides an interface to the DS' hardware.
 */
class Hardware {
public:

	/**
	 * Initialise the hardware.
	 */
	static void init();

	/**
	 * Shutdown the hardware.
	 */
	static void shutdown();

	/**
	 * Get a pointer to a Graphics object that can draw to the top screen.
	 * @return A pointer to a Graphics object that can draw to the top screen.
	 */
	static inline WoopsiGfx::Graphics* getTopGfx() {
		return _topGfx;
	};

	/**
	 * Get a pointer to a Graphics object that can draw to the bottom screen.
	 * @return A pointer to a Graphics object that can draw to the bottom
	 * screen.
	 */
	static inline WoopsiGfx::Graphics* getBottomGfx() {
		return _bottomGfx;
	};

	/**
	 * Get a reference to an object representing the state of the DS' buttons.
	  *@return A reference to the current pad state.
	 */
	static inline const Pad& getPad() {
		return _pad;
	};

	/**
	 * Get a reference to an object representing the state of the DS' stylus.
	  *@return A reference to the current stylus state.
	 */
	static inline const Stylus& getStylus() {
		return _stylus;
	};

	/**
	 * Waits for the next VBlank.  Also updates the pad/stylus states.
	 */
	static void waitForVBlank();

	/**
	 * Get a pointer to the SDLFrameBuffer object that wraps around the top
	 * frame buffer VRAM.
	 * @return A pointer to a SDLFrameBuffer object that wraps around the top
	 * frame buffer VRAM.
	 */
	static inline SDLFrameBuffer* getTopBuffer() { return _topBuffer; };

	/**
	 * Get a pointer to the SDLFrameBuffer object that wraps around the bottom
	 * frame buffer VRAM.
	 * @return A pointer to a SDLFrameBuffer object that wraps around the bottom
	 * frame buffer VRAM.
	 */
	static inline SDLFrameBuffer* getBottomBuffer() { return _bottomBuffer; };

private:
	static Pad _pad;						/**< State of the DS' pad. */
	static Stylus _stylus;					/**< State of the DS' stylus. */
	static SDLFrameBuffer* _topBuffer;		/**< Top frame buffer. */
	static SDLFrameBuffer* _bottomBuffer;	/**< Bottom frame buffer. */
	static WoopsiGfx::Graphics* _topGfx;	/**< Top display graphics object. */
	static WoopsiGfx::Graphics* _bottomGfx;	/**< Bottom display graphics object. */
    
#ifdef USING_SDL
    
	static SDL_Surface* _surface;			/**< SDL surface for visual output. */

#else

	static s32 _topBackgroundBase;			/**< Stores the base for the current top
												 background; used when double-buffering. */
	
#endif

	/**
	 * Constructor.
	 */
	Hardware() { };

	/**
	 * Destructor.
	 */
	~Hardware() { };
};

#endif
