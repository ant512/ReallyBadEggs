#ifndef _CONTROLLER_BASE_H_
#define _CONTROLLER_BASE_H_

/**
 * Base class for all controllers.  They abstract the controls needed for a
 * GridRunner object - left, right, down and rotation - into an interface.
 * Possible implementations of the interface include a class that watches the
 * physical DS buttons, a class that listens to the wifi interface for a remote
 * networked player, or an AI.
 */
class ControllerBase {
public:
	/**
	 * Constructor.
	 */
	ControllerBase() { };

	/**
	 * Destructor.
	 */
	virtual ~ControllerBase() { };

	/**
	 * Is the left control active?
	 * @return True if the left control is active.
	 */
	virtual bool left() = 0;

	/**
	 * Is the right control active?
	 * @return True if the right control is active.
	 */
	virtual bool right() = 0;

	/**
	 * Is the down control active?
	 * @return True if the down control is active.
	 */
	virtual bool down() = 0;

	/**
	 * Is the clockwise rotation control active?
	 * @return True if the clockwise rotation control is active.
	 */
	virtual bool rotateClockwise() = 0;

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	virtual bool rotateAntiClockwise() = 0;
};

#endif
