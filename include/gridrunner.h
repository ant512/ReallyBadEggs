#ifndef _GRID_RUNNER_H_
#define _GRID_RUNNER_H_

#include <graphics.h>
#include <nds.h>

#include "grid.h"
#include "controllerbase.h"

/**
 * Controls a grid.  Maintains a state machine that tracks what should happen
 * currently and next as the game progresses.
 */
class GridRunner {
public:

	/**
	 * Constructor.
	 * @param controller A controller object that will provide input for the
	 * movement of live blocks.
	 * @param blockColourCount Number of block colours available to the grid.
	 * @param startingHeight Number of rows that are filled with grey blocks at
	 * the start of the game.
	 */
	GridRunner(const ControllerBase* controller, s32 blockColourCount, s32 startingHeight);

	/**
	 * Destructor.
	 */
	~GridRunner();

	/**
	 * Process a single iteration of the state machine/grid logic.  This model
	 * enables other code to be run between iterations of the grid (for example,
	 * if two grids are running because we've got a two-player game).
	 * @param x The x co-ordinate to draw to.
	 * @param y The y co-ordinate to draw to.
	 * @param gfx The graphics object to draw with.
	 */
	void iterate(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

private:

	/**
	 * All possible states of the state machine.
	 */
	enum GridRunnerState {
		GRID_RUNNER_STATE_DROP = 0,			/**< Blocks are dropping automatically. */
		GRID_RUNNER_STATE_LIVE = 1,			/**< Live, user-controlled blocks are in play. */
		GRID_RUNNER_STATE_LANDING = 2,		/**< Blocks are running their landing animations. */
		GRID_RUNNER_STATE_EXPLODING = 3		/**< Blocks are running their exploding animations. */
	};

	static const s32 AUTO_DROP_TIME = 5;	/**< Frames between drops when blocks are automatically dropping. */
	static const s32 LIVE_DROP_TIME = 30;	/**< Frames between forced drops of live blocks. */

	Grid* _grid;							/**< The grid the runner controls. */
	GridRunnerState _state;					/**< The state of the state machine. */
	s32 _timer;								/**< Frames since the last event took place. */
	const ControllerBase* _controller;		/**< Controller that feeds user input to live blocks. */
};

#endif
