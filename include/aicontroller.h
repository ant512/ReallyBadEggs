#ifndef _AI_CONTROLLER_H_
#define _AI_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "grid.h"
#include "gridrunner.h"
#include "point.h"

/**
 * AI that plays the game.
 */
class AIController : public ControllerBase {
public:

	/**
	 * Constructor.
	 * @param hesitation Amount of hesitation to apply when making a decision to
	 * move.  Larger values == greater chance of hesitation.
	 */
	AIController(s32 hesitation);

	/**
	 * Destructor.
	 */
	~AIController() { };

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

	const GridRunner* _gridRunner;			/**< The GridRunner that the AI is controlling. */
	s32 _lastLiveBlockY;					/**< The last observed y co-ordinate of the first live block. */
	s32 _targetX;							/**< The x co-ordinate the AI is trying to move the live block to. */
	s32 _targetRotations;					/**< Number of clockwise rotations needed before correct live block
												 orientation is achieved. */
	s32 _hesitation;						/**< Chance that the AI will hesitate (larger value = more likely;
												 0 = no hesitation). */

	/**
	 * Get a score for the proposed co-ordinates of the supplied blocks.
	 * @param block1 First block in the shape.
	 * @param block2 Second block in the shape.
	 * @param point1 The co-ordinates of the first block.
	 * @param point2 The co-ordinates of the second block.
	 */
	s32 scoreShapePosition(BlockBase* block1, BlockBase* block2, const Point& point1, const Point& point2);
};

#endif
