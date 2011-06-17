#ifndef _AI_CONTROLLER_H_
#define _AI_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
#include "grid.h"
#include "gridrunner.h"

/**
 * AI that plays the game.
 */
class AIController : public ControllerBase {
public:

	/**
	 * Constructor.
	 * @param gridRunner The GridRunner that the AI is controlling.
	 */
	AIController() {
		_gridRunner = NULL;
	};

	/**
	 * Destructor.
	 */
	~AIController() { };

	void setGridRunner(const GridRunner* gridRunner) {
		_gridRunner = gridRunner;
	};

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
	const GridRunner* _gridRunner;	/**< The GridRunner that the AI is controlling. */
};

#endif
