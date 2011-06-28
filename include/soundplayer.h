#ifndef _SOUND_PLAYER_H_
#define _SOUND_PLAYER_H_

#include <nds.h>

#ifndef USING_SDL

#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#else

#include <SDL_mixer/SDL_mixer.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#include <woopsistring.h>

#endif

/**
 * Manages all sounds within the game.
 */
class SoundPlayer {
public:

	/**
	 * Initialises all game sounds.
	 */
	static void init() {

#ifndef USING_SDL

		mmInitDefaultMem((mm_addr)soundbank_bin);
		mmLoadEffect(SFX_PAUSE);
		
#else
		
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S8, MIX_DEFAULT_CHANNELS, 512);		
		Mix_Volume(-1, 60);

#endif
	};

	/**
	 * Plays the game paused sound.
	 */
	static void playPause() {
#ifndef USING_SDL
		mmEffect(SFX_PAUSE);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_PAUSE], 0);
#endif
	};

	/**
	 * Shuts down all game sounds.
	 */
	static void shutdown() {
		
#ifndef USING_SDL
		
		mmUnloadEffect(SFX_PAUSE);
		
#else
		
		for (u32 i = 0; i < SOUND_TYPE_COUNT; i++) {
			Mix_FreeChunk(_sounds[i]);
		}
		
		Mix_CloseAudio();
		
#endif
		
	};

private:
#ifndef USING_SDL
	
#else
	
	static const u32 SOUND_TYPE_COUNT = 1;
	
	enum SFXType {
		SFX_PAUSE = 0
	};
	
	enum ChannelType {
		CHANNEL_SFX = -1,
		CHANNEL_MUSIC = 0
	};
	
	static Mix_Chunk* _sounds[SOUND_TYPE_COUNT];

	static void loadWav(SFXType type, const char* fileName) {
		
		// Get path of executable
		char path[1024];
		uint32_t size = sizeof(path);
		_NSGetExecutablePath(path, &size);
		
		WoopsiGfx::WoopsiString str(path);
		
		// Strip the name of the executable
		s32 lastIndex = str.lastIndexOf('/');
		str = str.subString(0, lastIndex);
		
		// Add the name of the wav
		str.append("/");
		str.append(fileName);
		
		char* buffer = new char[str.getByteCount() + 1];
		str.copyToCharArray(buffer);
		
		_sounds[type] = Mix_LoadWAV(buffer);
		
		delete[] buffer;
	};

#endif

	/**
	 * Constructor.
	 */
	SoundPlayer() { };

	/**
	 * Destructor.
	 */
	~SoundPlayer() { };
};

#endif