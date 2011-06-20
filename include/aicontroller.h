#ifndef _AI_CONTROLLER_H_
#define _AI_CONTROLLER_H_

#include <nds.h>

#include "controllerbase.h"
#include "hardware.h"
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
	 * @param gridRunner The GridRunner that the AI is controlling.
	 */
	AIController() {
		_gridRunner = NULL;
		_lastLiveBlockY = Grid::GRID_HEIGHT;
		_targetX = 0;
	};

	/**
	 * Destructor.
	 */
	~AIController() { };

	void analyseGrid() {

		const Grid* grid = _gridRunner->getGrid();

		Point liveBlock1;
		Point liveBlock2;

		grid->getLiveBlockPoints(liveBlock1, liveBlock2);

		// If last observed y is greater than current live block y, we'll need
		// to choose a new move
		if (_lastLiveBlockY > liveBlock1.y) {

			// Get the y co-ords of the topmost blank block in each column
			s32 columnY[6];

			for (s32 i = 0; i < 6; ++i) {
				columnY[i] = (Grid::GRID_HEIGHT - grid->getColumnHeight(i));
			}

			s32 bestScore = 0;
			s32 bestScoreX = 0;

			// Get the score for each possible move we can make.  Assumptions:
			// - We never rotate a block;
			// - There is nothing to stop us moving a block (ie. no filled
			//   columns).
			for (s32 i = 0; i < 5; ++i) {

				// Chain lengths
				s32 chainLength = grid->getPotentialChainLength(i, columnY[i], grid->getBlockAt(liveBlock1.x, liveBlock1.y));
				chainLength += grid->getPotentialChainLength(i, columnY[i], grid->getBlockAt(liveBlock2.x, liveBlock2.y));

				// TODO: Penalty for increasing column height

				s32 score = chainLength;

				if (score > bestScore) {
					bestScore = score;
					bestScoreX = i;
				}
			}

			// Get a new target column
			_targetX = bestScoreX;
		}

		// Remember the co-ord of the block in the grid so that next time we
		// can identify if we've got a new live block or not
		_lastLiveBlockY = liveBlock1.y;
	};

	void setGridRunner(const GridRunner* gridRunner) {
		_gridRunner = gridRunner;
	};

	/**
	 * Is the left control active?
	 * @return True if the left control is active.
	 */
	bool left() {
		analyseGrid();

		const Grid* grid = _gridRunner->getGrid();

		Point liveBlock1;
		Point liveBlock2;

		grid->getLiveBlockPoints(liveBlock1, liveBlock2);
		
		return (liveBlock1.x > _targetX);
	};

	/**
	 * Is the right control active?
	 * @return True if the right control is active.
	 */
	bool right() {
		analyseGrid();

		const Grid* grid = _gridRunner->getGrid();

		Point liveBlock1;
		Point liveBlock2;

		grid->getLiveBlockPoints(liveBlock1, liveBlock2);
		
		return (liveBlock1.x < _targetX);
	};

	/**
	 * Is the down control active?
	 * @return True if the down control is active.
	 */
	bool down() {
		analyseGrid();

		const Grid* grid = _gridRunner->getGrid();

		Point liveBlock1;
		Point liveBlock2;

		grid->getLiveBlockPoints(liveBlock1, liveBlock2);
		
		return (liveBlock1.x == _targetX);
	};

	/**
	 * Is the clockwise rotation control active?
	 * @return True if the clockwise rotation control is active.
	 */
	bool rotateClockwise() {
		return false;
	};

	/**
	 * Is the anticlockwise rotation control active?
	 * @return True if the anticlockwise rotation control is active.
	 */
	bool rotateAntiClockwise() {
		return false;
	};

private:
	const GridRunner* _gridRunner;	/**< The GridRunner that the AI is controlling. */
	s32 _lastLiveBlockY;
	s32 _targetX;
};

#endif
