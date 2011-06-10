#include <nds.h>

#include "hardware.h"

int main(int argc, char* argv[]) {
	Hardware::init();


	Hardware::shutdown();

	return 0;
}
