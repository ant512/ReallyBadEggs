#ifndef _PLAYER_1_CONTROLLER_H_
#define _PLAYER_1_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
#include "pad.h"

/**
 * Watches the DS' buttons and reports their states appropriately.
 */
class Player1Controller : public ControllerBase {
public:

	/**
	 * Constructor.
	 */
	Player1Controller() { };

	/**
	 * Destructor.
	 */
	~Player1Controller() { };

	/**
	 * Is the left control active?
	 * @return True if the left control is active.
	 */
	bool left() {
		const Pad& pad = Hardware::getPad();
		return pad.isLeftNewPress() || pad.isLeftRepeat();
	};

	/**
	 * Is the right control active?
	 * @return True if the right control is active.
	 */
	bool right() {
		const Pad& pad = Hardware::getPad();
		return pad.isRightNewPress() || pad.isRightRepeat();
	};

	/**
	 * Is the down control active?
	 * @return True if the down control is active.
	 */
	bool down() {
		const Pad& pad = Hardware::getPad();
		return pad.isDownHeld();
	};

	/**
	 * Is the clockwise rotation control active?
	 * @return True if the clockwise rotation control is active.
	 */
	bool rotateClockwise() {
		const Pad& pad = Hardware::getPad();
		return pad.isLNewPress();
	};

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	bool rotateAntiClockwise() {
		const Pad& pad = Hardware::getPad();
		return pad.isUpNewPress();
	};
};

#endif
