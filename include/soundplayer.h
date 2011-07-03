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
		mmLoadEffect(SFX_CHAIN);
		mmLoadEffect(SFX_DEAD);
		mmLoadEffect(SFX_DROP);
		mmLoadEffect(SFX_GARBAGE);
		mmLoadEffect(SFX_GARBAGEBIG);
		mmLoadEffect(SFX_LAND);
		mmLoadEffect(SFX_LOSE);
		mmLoadEffect(SFX_MOVE);
		mmLoadEffect(SFX_MULTICHAIN1);
		mmLoadEffect(SFX_MULTICHAIN2);
		mmLoadEffect(SFX_PAUSE);
		mmLoadEffect(SFX_ROTATE);
		mmLoadEffect(SFX_WIN);

		mmLoad(MOD_TITLE);

		mmSetModuleVolume(512);
		
#else
		
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S8, MIX_DEFAULT_CHANNELS, 512);		
		Mix_Volume(-1, 60);

		// Load sfx from resources folder
		loadWav(SFX_CHAIN, "../Resources/chain.wav");
		loadWav(SFX_DEAD, "../Resources/dead.wav");
		loadWav(SFX_DROP, "../Resources/drop.wav");
		loadWav(SFX_GARBAGE, "../Resources/garbage.wav");
		loadWav(SFX_GARBAGEBIG, "../Resources/garbagebig.wav");
		loadWav(SFX_LAND, "../Resources/land.wav");
		loadWav(SFX_LOSE, "../Resources/lose.wav");
		loadWav(SFX_MOVE, "../Resources/move.wav");
		loadWav(SFX_MULTICHAIN1, "../Resources/multichain1.wav");
		loadWav(SFX_MULTICHAIN2, "../Resources/multichain2.wav");
		loadWav(SFX_PAUSE, "../Resources/pause.wav");
		loadWav(SFX_ROTATE, "../Resources/rotate.wav");
		loadWav(SFX_WIN, "../Resources/win.wav");
#endif
	};

	static void playChain(s32 player, s32 scoreMultiplier) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_CHAIN;
		sound.rate = 1024 | (scoreMultiplier << 6);
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_CHAIN], 0);
#endif
	};

	static void playMenuBack() {
#ifndef USING_SDL
		mmEffect(SFX_DROP);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_DROP], 0);
#endif
	};

	static void playDead() {
#ifndef USING_SDL
		mmEffect(SFX_DEAD);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_DEAD], 0);
#endif
	};

	static void playDrop(s32 player) {

		// Don't play drop sound for CPU as it is maddeningly annoying
		if (player == 1) return;

#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_DROP;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_DROP], 0);
#endif
	};

	static void playGarbage(s32 player) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_GARBAGE;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_GARBAGE], 0);
#endif
	};

	static void playGarbageBig(s32 player) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_GARBAGEBIG;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_GARBAGEBIG], 0);
#endif
	};

	static void playLand(s32 player) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_LAND;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_LAND], 0);
#endif
	};

	static void playMenuMove() {
#ifndef USING_SDL
		mmEffect(SFX_MOVE);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_MOVE], 0);
#endif
	};

	static void playMove(s32 player) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_MOVE;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_MOVE], 0);
#endif
	};

	static void playMultichain(s32 player) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = player == 0 ? SFX_MULTICHAIN1 : SFX_MULTICHAIN2;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[player == 0 ? SFX_MULTICHAIN1 : SFX_MULTICHAIN2], 0);
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
	 * Plays the menu selection sound.
	 */
	static void playMenuSelect() {
#ifndef USING_SDL
		mmEffect(SFX_ROTATE);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_ROTATE], 0);
#endif
	};

	/**
	 * Plays the block rotation sound.
	 */
	static void playRotate(s32 player) {
#ifndef USING_SDL

		mm_sound_effect sound;
		sound.id = SFX_ROTATE;
		sound.rate = 1024;
		sound.handle = 0;
		sound.volume = 255;
		sound.panning = player * 255;

		mmEffectEx(&sound);

#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_ROTATE], 0);
#endif
	};

	static void playLose() {
#ifndef USING_SDL
		mmEffect(SFX_LOSE);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_LOSE], 0);
#endif
	};

	static void playWin() {
#ifndef USING_SDL
		mmEffect(SFX_WIN);
#else
		Mix_PlayChannel(CHANNEL_SFX, _sounds[SFX_WIN], 0);
#endif
	};

	static void playTitleMusic() {
#ifndef USING_SDL
		mmStart(MOD_TITLE, MM_PLAY_LOOP);
#endif
	};

	static void stopMusic() {
#ifndef USING_SDL
		mmStop();
#endif
	};

	/**
	 * Shuts down all game sounds.
	 */
	static void shutdown() {
		
#ifndef USING_SDL
		
		mmUnloadEffect(SFX_CHAIN);
		mmUnloadEffect(SFX_DEAD);
		mmUnloadEffect(SFX_DROP);
		mmUnloadEffect(SFX_GARBAGE);
		mmUnloadEffect(SFX_GARBAGEBIG);
		mmUnloadEffect(SFX_LAND);
		mmUnloadEffect(SFX_LOSE);
		mmUnloadEffect(SFX_MOVE);
		mmUnloadEffect(SFX_MULTICHAIN1);
		mmUnloadEffect(SFX_MULTICHAIN2);
		mmUnloadEffect(SFX_PAUSE);
		mmUnloadEffect(SFX_ROTATE);
		mmUnloadEffect(SFX_WIN);

		mmUnload(MOD_TITLE);
		
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
	
	static const u32 SOUND_TYPE_COUNT = 13;
	
	enum SFXType {
		SFX_CHAIN = 0,
		SFX_DEAD = 1,
		SFX_DROP = 2,
		SFX_GARBAGE = 3,
		SFX_GARBAGEBIG = 4,
		SFX_LAND = 5,
		SFX_LOSE = 6,
		SFX_MOVE = 7,
		SFX_MULTICHAIN1 = 8,
		SFX_MULTICHAIN2 = 9,
		SFX_PAUSE = 10,
		SFX_ROTATE = 11,
		SFX_WIN = 12
	};
	
	enum ChannelType {
		CHANNEL_SFX = -1
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
