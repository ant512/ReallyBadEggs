#ifndef _PLAYER_CONTROLLER_H_
#define _PLAYER_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
#include "pad.h"

/**
 * Watches the DS' buttons and reports their states appropriately.
 */
class PlayerController : public ControllerBase {
public:

	/**
	 * Constructor.
	 */
	PlayerController() { };

	/**
	 * Destructor.
	 */
	~PlayerController() { };

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
		return pad.isANewPress();
	};

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	bool rotateAntiClockwise() {
		const Pad& pad = Hardware::getPad();
		return pad.isBNewPress();
	};
};

#endif
