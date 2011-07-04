#include <nds.h>

#include "game.h"
#include "hardware.h"
#include "soundplayer.h"

int main(int argc, char* argv[]) {

	Hardware::init();
	SoundPlayer::init();

	Game* game = new Game();
	game->main();

	delete game;

	SoundPlayer::shutdown();
	Hardware::shutdown();
	return 0;
}
