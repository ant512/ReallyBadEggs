#ifndef _SMART_AI_CONTROLLER_H_
#define _SMART_AI_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "grid.h"
#include "gridrunner.h"
#include "point.h"

/**
 * AI that plays the game.
 */
class SmartAIController : public ControllerBase {
public:

	/**
	 * Constructor.
	 * @param gridRunner The GridRunner that the AI is controlling.
	 */
	SmartAIController();

	/**
	 * Destructor.
	 */
	~SmartAIController() { };

	/**
	 * Decides where to move the current live block.  Checks the state of the
	 * grid and works out how many points are given from landing the current
	 * block at each possible location without rotating it.
	 */
	void analyseGrid();

	/**
	 * Sets the grid runner that the controller will control.
	 * @param gridRunner The grid runner that the controller will control.
	 */
	void setGridRunner(const GridRunner* gridRunner);

	/**
	 * Is the left control active?
	 * @return True if the left control is active.
	 */
	bool left();

	/**
	 * Is the right control active?
	 * @return True if the right control is active.
	 */
	bool right();

	/**
	 * Is the down control active?
	 * @return True if the down control is active.
	 */
	bool down();

	/**
	 * Is the clockwise rotation control active?
	 * @return True if the clockwise rotation control is active.
	 */
	bool rotateClockwise();

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	bool rotateAntiClockwise();

private:
	const GridRunner* _gridRunner;	/**< The GridRunner that the AI is controlling. */
	s32 _lastLiveBlockY;			/**< The last observed y co-ordinate of the first live block. */
	s32 _targetX;					/**< The x co-ordinate the AI is trying to move the live block to. */
	s32 _targetRotations;
};

#endif
