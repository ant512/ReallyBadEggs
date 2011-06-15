#include "gridrunner.h"
#include "hardware.h"

GridRunner::GridRunner(const ControllerBase* controller,
					   s32 startingHeight,
					   BlockServer* blockServer,
					   s32 playerNumber) {

	_state = GRID_RUNNER_STATE_DROP;
	_timer = 0;
	_controller = controller;
	_grid = new Grid(startingHeight);
	_blockServer = blockServer;
	_playerNumber = playerNumber;

	_nextBlocks = new BlockBase*[2];

	// Ensure we have some initial blocks to add to the grid
	_blockServer->getNextBlocks(_playerNumber, &_nextBlocks[0], &_nextBlocks[1]);
}

GridRunner::~GridRunner() {
	delete _grid;

	// Delete the next blocks
	for (s32 i = 0; i < 2; ++i) {
		if (_nextBlocks[i] != NULL) {
			delete _nextBlocks[i];
			_nextBlocks[i] = NULL;
		}
	}

	delete[] _nextBlocks;
}

void GridRunner::renderNextBlocks(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	s32 renderX = 0;

	for (s32 i = 0; i < 2; ++i) {
		renderX = x + (i * Grid::BLOCK_SIZE);

		BlockBase* block = _nextBlocks[i];

		if (block == NULL) {
			gfx->drawFilledRect(renderX, y, Grid::BLOCK_SIZE, Grid::BLOCK_SIZE, woopsiRGB(0, 0, 0));
		} else {
			block->render(renderX, y, gfx);
		}
	}
}

void GridRunner::iterate(s32 x, s32 y, WoopsiGfx::Graphics* gfx) {

	// Returns true if any blocks have an animation still in progress
	bool animated = _grid->animate();
	_grid->render(x, y, gfx);

	renderNextBlocks(x + ((Grid::GRID_WIDTH + 1) * Grid::BLOCK_SIZE), y, gfx);

	++_timer;

	switch (_state) {
		case GRID_RUNNER_STATE_DROP:

			// Blocks are dropping down the screen automatically

			if (_timer < AUTO_DROP_TIME) return;

			_timer = 0;

			if (!_grid->dropBlocks()) {

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
				_grid->connectBlocks();

				// Attempt to explode any chains that exist in the grid
				if (_grid->explodeChains()) {

					// We need to run the explosion animations next
					_state = GRID_RUNNER_STATE_EXPLODING;
				} else {

					// Nothing exploded, so we can put a new live block into
					// the grid
					_grid->addLiveBlocks(_nextBlocks[0], _nextBlocks[1]);

					// Fetch the next blocks from the block server and remember
					// them
					_blockServer->getNextBlocks(_playerNumber, &_nextBlocks[0], &_nextBlocks[1]);

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

			if (_grid->hasLiveBlocks()) {

				bool dropped = false;

				// Drop the block to the next row if the timer has expired
				if (_timer == LIVE_DROP_TIME) {
					_timer = 0;
					
					// Only force blocks down when player is not doing it
					_grid->dropLiveBlocks();

					dropped = true;
				}

				// Process user input
				if (_controller->left()) {
					_grid->moveLiveBlocksLeft();
				} else if (_controller->right()) {
					_grid->moveLiveBlocksRight();
				} else if (_controller->down() && (_timer % 2 == 0) && !dropped) {
					_grid->dropLiveBlocks();
				} else if (_controller->rotateClockwise()) {
					_grid->rotateLiveBlocksClockwise();
				} else if (_controller->rotateAntiClockwise()) {
					_grid->rotateLiveBlocksAntiClockwise();
				}
			} else {

				// At least one of the blocks in the live pair has touched down.
				// We need to drop the other block automatically
				_state = GRID_RUNNER_STATE_DROP;
			}

			break;	
	}
}
