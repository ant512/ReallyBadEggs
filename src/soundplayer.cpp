#include "soundplayer.h"

#ifndef USING_SDL

#else

Mix_Chunk* SoundPlayer::_sounds[SOUND_TYPE_COUNT];

#endif
