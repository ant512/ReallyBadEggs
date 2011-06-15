#include "gridrunner.h"
#include "hardware.h"

GridRunner::GridRunner(const ControllerBase* controller) {
	_state = GRID_RUNNER_STATE_DROP;
	_timer = 0;
	_controller = controller;
}

GridRunner::~GridRunner() {
}

void GridRunner::iterate() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	// Returns true if any blocks have an animation still in progress
	bool animated = _grid.animate();
	_grid.render(0, 0, gfx);

	++_timer;

	switch (_state) {
		case GRID_RUNNER_STATE_DROP:

			// Blocks are dropping down the screen automatically

			if (_timer < AUTO_DROP_TIME) return;

			_timer = 0;

			if (!_grid.dropBlocks()) {

				// Blocks have stopped dropping, so we need to run the landing
				// animations
				_state = GRID_RUNNER_STATE_LANDING;
			}

			break;
		
		case GRID_RUNNER_STATE_LANDING:

			// Block landing animations are running

			if (!animated) {

				// All animations have finished, so establish all connections
				// between blocks now that they have settled
				_grid.connectBlocks();

				// Attempt to explode any chains that exist in the grid
				if (_grid.explodeChains()) {

					// We need to run the explosion animations next
					_state = GRID_RUNNER_STATE_EXPLODING;
				} else {

					// Nothing exploded, so we can put a new live block into
					// the grid
					_grid.addLiveBlocks();

					_state = GRID_RUNNER_STATE_LIVE;
				}
			}

			break;

		case GRID_RUNNER_STATE_EXPLODING:

			// Block explosion animations are running

			if (!animated) {

				// All animations have finished - we need to drop any blocks
				// that are now sat on holes in the grid
				_state = GRID_RUNNER_STATE_DROP;
			}

			break;

		case GRID_RUNNER_STATE_LIVE:

			// Player-controllable blocks are in the grid

			if (_grid.hasLiveBlocks()) {

				// Drop the block to the next row if the timer has expired
				if (_timer == LIVE_DROP_TIME) {
					_timer = 0;
					_grid.dropLiveBlocks();
				}

				// Process user input
				if (_controller->left()) {
					_grid.moveLiveBlocksLeft();
				} else if (_controller->right()) {
					_grid.moveLiveBlocksRight();
				} else if (_controller->down()) {
					_grid.dropLiveBlocks();
				} else if (_controller->rotateClockwise()) {
					_grid.rotateLiveBlocksClockwise();
				} else if (_controller->rotateAntiClockwise()) {
					_grid.rotateLiveBlocksAntiClockwise();
				}
			} else {

				// At least one of the blocks in the live pair has touched down.
				// We need to drop the other block automatically
				_state = GRID_RUNNER_STATE_DROP;
			}

			break;	
	}
}
