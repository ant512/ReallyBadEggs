#ifndef _AI_CONTROLLER_H_
#define _AI_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
#include "grid.h"


/**
 * AI that plays the game.
 */
class AIController : public ControllerBase {
public:

	/**
	 * Constructor.
	 */
	AIController(const Grid* grid) {
		_grid = grid;
	};

	/**
	 * Destructor.
	 */
	~AIController() { };

	/**
	 * Is the left control active?
	 * @return True if the left control is active.
	 */
	bool left() const {
		return rand() % 20 == 0;
	};

	/**
	 * Is the right control active?
	 * @return True if the right control is active.
	 */
	bool right() const {
		return rand() % 20 == 0;
	};

	/**
	 * Is the down control active?
	 * @return True if the down control is active.
	 */
	bool down() const {
		return rand() % 40 == 0;
	};

	/**
	 * Is the clockwise rotation control active?
	 * @return True if the clockwise rotation control is active.
	 */
	bool rotateClockwise() const {
		return rand() % 80 == 0;
	};

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	bool rotateAntiClockwise() const {
		return rand() % 80 == 0;
	};

private:
	const Grid* _grid;
};

#endif
