#include <woopsistring.h>

#include "gridrunner.h"
#include "hardware.h"

GridRunner::GridRunner(const ControllerBase* controller,
					   Grid* grid,
					   BlockServer* blockServer,
					   s32 playerNumber,
					   s32 x) {

	_state = GRID_RUNNER_STATE_DROP;
	_timer = 0;
	_controller = controller;
	_grid = grid;
	_blockServer = blockServer;
	_playerNumber = playerNumber;
	_x = x;

	_score = 0;
	_level = 0;
	_chains = 0;
	_scoreMultiplier = 0;
	_outgoingGreyBlockCount = 0;
	_pendingGreyBlockCount = 0;

	_nextBlocks = new BlockBase*[2];

	// Ensure we have some initial blocks to add to the grid
	_blockServer->getNextBlocks(_playerNumber, &_nextBlocks[0], &_nextBlocks[1]);

	renderScore(_x, 16);
	renderLevelNumber(_x, 24);
	renderChainCount(_x, 32);
	renderIncomingGarbage(_x, 40);
	renderOutgoingGarbage(_x, 48);
}

GridRunner::~GridRunner() {

	// Delete the next blocks
	for (s32 i = 0; i < 2; ++i) {
		if (_nextBlocks[i] != NULL) {
			delete _nextBlocks[i];
			_nextBlocks[i] = NULL;
		}
	}

	delete[] _nextBlocks;
}

void GridRunner::renderScore(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str;
	str.format("%06d", _score);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderLevelNumber(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str;
	str.format("%02d", _level);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderChainCount(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str;
	str.format("%04d", _chains);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderOutgoingGarbage(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str;
	str.format("%02d", _outgoingGreyBlockCount);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderIncomingGarbage(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str;
	str.format("%02d", _pendingGreyBlockCount);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderNextBlocks(s32 x, s32 y) const {

	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

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

void GridRunner::iterate() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	// Returns true if any blocks have an animation still in progress
	bool animated = _grid->animate();
	_grid->render(_x, 0, gfx);

	++_timer;


	renderIncomingGarbage(_x, 40);
	renderOutgoingGarbage(_x, 48);


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

				s32 score = 0;
				s32 chains = 0;

				// Attempt to explode any chains that exist in the grid
				if (_grid->explodeChains(score, chains)) {
					
					++_scoreMultiplier;

					_score += score * _scoreMultiplier;
					_chains += chains;
					_level = _chains / 10;

					_outgoingGreyBlockCount += (score * _scoreMultiplier) / (Grid::CHAIN_LENGTH * Grid::BLOCK_EXPLODE_SCORE);

					renderScore(_x, 16);
					renderLevelNumber(_x, 24);
					renderChainCount(_x, 32);

					// We need to run the explosion animations next
					_state = GRID_RUNNER_STATE_EXPLODING;

				} else if (_pendingGreyBlockCount > 0) {

					if (_outgoingGreyBlockCount > 0) {
						_pendingGreyBlockCount -= _outgoingGreyBlockCount;

						if (_pendingGreyBlockCount < 0) {

							_outgoingGreyBlockCount = -_pendingGreyBlockCount;
							_pendingGreyBlockCount = 0;
						} else {
							_outgoingGreyBlockCount = 0;
						}
					}

					// Add any incoming grey blocks
					_grid->addGarbage(_pendingGreyBlockCount);
					_pendingGreyBlockCount = 0;

					// Switch back to the drop state
					_state = GRID_RUNNER_STATE_DROP;
				} else {

					// Nothing exploded, so we can put a new live block into
					// the grid
					_grid->addLiveBlocks(_nextBlocks[0], _nextBlocks[1]);

					// Fetch the next blocks from the block server and remember
					// them
					_blockServer->getNextBlocks(_playerNumber, &_nextBlocks[0], &_nextBlocks[1]);

					renderNextBlocks(_x + ((Grid::GRID_WIDTH - 2) * Grid::BLOCK_SIZE / 2), 0);

					_scoreMultiplier = 0;

					// Queue up outgoing blocks for the other player
					if (_outgoingGreyBlockCount > 0) {
						_pendingGreyBlockCount -= _outgoingGreyBlockCount;
						_outgoingGreyBlockCount = 0;
					}

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
				s32 timeToDrop = LIVE_DROP_TIME - _level;
				if (timeToDrop < 0) timeToDrop = 0;

				if (_timer >= timeToDrop) {
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

s32 GridRunner::getOutgoingGreyBlockCount() const {
	if (_pendingGreyBlockCount > 0) return 0;
	return 0 - _pendingGreyBlockCount;
}

s32 GridRunner::getIncomingGreyBlockCount() const {
	if (_pendingGreyBlockCount < 0) return 0;
	return _pendingGreyBlockCount;
}

const Grid* GridRunner::getGrid() const {
	return _grid;
}

void GridRunner::addIncomingGreyBlocks(s32 count) {
	_pendingGreyBlockCount += count;
}

void GridRunner::clearOutgoingGreyBlockCount() {
	if (_pendingGreyBlockCount < 0) _pendingGreyBlockCount = 0;
}

bool GridRunner::canReceiveGarbage() const {
	return _state == GRID_RUNNER_STATE_LIVE;
}
