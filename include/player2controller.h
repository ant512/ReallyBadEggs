#ifndef _PLAYER_2_CONTROLLER_H_
#define _PLAYER_2_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
#include "pad.h"

/**
 * Watches the DS' buttons and reports their states appropriately.
 */
class Player2Controller : public ControllerBase {
public:

	/**
	 * Constructor.
	 */
	Player2Controller() { };

	/**
	 * Destructor.
	 */
	~Player2Controller() { };

	/**
	 * Is the left control active?
	 * @return True if the left control is active.
	 */
	bool left() {
		const Pad& pad = Hardware::getPad();
		return pad.isYNewPress() || pad.isYRepeat();
	};

	/**
	 * Is the right control active?
	 * @return True if the right control is active.
	 */
	bool right() {
		const Pad& pad = Hardware::getPad();
		return pad.isANewPress() || pad.isARepeat();
	};

	/**
	 * Is the down control active?
	 * @return True if the down control is active.
	 */
	bool down() {
		const Pad& pad = Hardware::getPad();
		return pad.isBHeld();
	};

	/**
	 * Is the clockwise rotation control active?
	 * @return True if the clockwise rotation control is active.
	 */
	bool rotateClockwise() {
		const Pad& pad = Hardware::getPad();
		return pad.isRNewPress();
	};

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	bool rotateAntiClockwise() {
		const Pad& pad = Hardware::getPad();
		return pad.isXNewPress();
	};
};

#endif
