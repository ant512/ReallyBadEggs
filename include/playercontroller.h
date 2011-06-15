#ifndef _PLAYER_CONTROLLER_H_
#define _PLAYER_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
#include "pad.h"

class PlayerController : public ControllerBase {
public:
	PlayerController() { };

	virtual ~PlayerController() { };

	bool left() const {
		const Pad& pad = Hardware::getPad();
		return pad.isLeftNewPress() || pad.isLeftRepeat();
	};

	bool right() const {
		const Pad& pad = Hardware::getPad();
		return pad.isRightNewPress() || pad.isRightRepeat();
	};

	bool down() const {
		const Pad& pad = Hardware::getPad();
		return pad.isDownNewPress() || pad.isDownRepeat();
	};

	bool rotateClockwise() const {
		const Pad& pad = Hardware::getPad();
		return pad.isANewPress();
	};

	bool rotateAntiClockwise() const {
		const Pad& pad = Hardware::getPad();
		return pad.isBNewPress();
	};
};

#endif
