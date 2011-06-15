#include "gridrunner.h"
#include "hardware.h"

GridRunner::GridRunner() {
	_state = GRID_RUNNER_STATE_DROP;
	_timer = 0;
}

GridRunner::~GridRunner() {
}

void GridRunner::iterate() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	// Returns true if any blocks have an animation still in progress
	bool animated = _grid.animate();
	_grid.render(0, 0, gfx);

	switch (_state) {
		case GRID_RUNNER_STATE_DROP:

			// Blocks are dropping down the screen automatically

			// TODO: 5 frames before next drop attempt
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
				const Pad& pad = Hardware::getPad();

				if (pad.isLeftNewPress() || pad.isLeftRepeat()) {
					_grid.moveLiveBlocksLeft();
				} else if (pad.isRightNewPress() || pad.isRightRepeat()) {
					_grid.moveLiveBlocksRight();
				} else if (pad.isDownNewPress() || pad.isDownRepeat()) {
					_grid.dropLiveBlocks();
				} else if (pad.isANewPress()) {
					_grid.rotateLiveBlocksClockwise();
				} else if (pad.isBNewPress()) {
					_grid.rotateLiveBlocksAntiClockwise();
				}
			} else {

				// At least one of the blocks in the live pair has touched down.
				// We need to drop the other
				_state = GRID_RUNNER_STATE_DROP;
			}

			break;	
	}
}
