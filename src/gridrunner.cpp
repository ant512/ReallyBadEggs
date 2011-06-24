#include <woopsistring.h>

#include "gridrunner.h"
#include "hardware.h"

// Hard-coded lookup list for level speeds.  The speed increase between each
// levels 0 to 10 is larger than the increase between levels 11 to 19 as the
// increase becomes more significant as the wait period gets shorter
const s32 GridRunner::LEVEL_SPEEDS[20] = { 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

GridRunner::GridRunner(ControllerBase* controller,
					   Grid* grid,
					   BlockServer* blockServer,
					   s32 playerNumber,
					   s32 x,
					   GameType gameType) {

	_state = GRID_RUNNER_STATE_DROP;
	_timer = 0;
	_controller = controller;
	_grid = grid;
	_blockServer = blockServer;
	_playerNumber = playerNumber;
	_x = x;
	_gameType = gameType;

	_score = 0;
	_level = 0;
	_chains = _gameType == GAME_TYPE_B ? GAME_TYPE_B_START_CHAINS : 0;
	_scoreMultiplier = 0;
	_outgoingGarbageCount = 0;
	_pendingGarbageCount = 0;

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

	// Indicator is disabled in two-player games
	if (_gameType == GAME_TYPE_TWO_PLAYER) return;

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
	str.format("%02d", _outgoingGarbageCount);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderIncomingGarbage(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str;
	str.format("%02d", _pendingGarbageCount);

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

void GridRunner::drop() {

	// Blocks are dropping down the screen automatically

	if (_timer < AUTO_DROP_TIME) return;

	_timer = 0;

	if (!_grid->dropBlocks()) {

		// Blocks have stopped dropping, so we need to run the landing
		// animations
		_state = GRID_RUNNER_STATE_LANDING;
	}
}

void GridRunner::land() {

	// All animations have finished, so establish connections between blocks now
	// that they have landed
	_grid->connectBlocks();

	s32 score = 0;
	s32 chains = 0;
	s32 blocks = 0;

	// Attempt to explode any chains that exist in the grid
	if (_grid->explodeChains(score, chains, blocks)) {
		
		++_scoreMultiplier;

		_score += score * _scoreMultiplier;

		switch (_gameType) {
			case GAME_TYPE_A:
				_chains += chains;
				_level = _chains / 10;
				break;
			case GAME_TYPE_B:
				_chains -= chains;
				break;
			case GAME_TYPE_TWO_PLAYER:
				_chains += chains;

				s32 garbage = 0;

				if (_scoreMultiplier == 1) {

					// One block for the chain and one block for each block on
					// top of the required minimum number
					garbage = blocks - (Grid::CHAIN_LENGTH - 1);
				} else {

					// If we're in a sequence of chains, we add 6 blocks each
					// sequence
					garbage = CHAIN_SEQUENCE_GARBAGE_BONUS;

					// Add any additional blocks on top of the standard
					// chain length
					garbage += blocks - Grid::CHAIN_LENGTH;
				}

				_outgoingGarbageCount += garbage;
				break;
		}

		renderScore(_x, 16);
		renderLevelNumber(_x, 24);
		renderChainCount(_x, 32);

		// We need to run the explosion animations next
		_state = GRID_RUNNER_STATE_EXPLODING;

	} else if (_pendingGarbageCount > 0) {

		// Add any incoming garbage blocks
		if (_outgoingGarbageCount > 0) {
			_pendingGarbageCount -= _outgoingGarbageCount;

			if (_pendingGarbageCount < 0) {

				_outgoingGarbageCount = -_pendingGarbageCount;
				_pendingGarbageCount = 0;
			} else {
				_outgoingGarbageCount = 0;
			}
		}

		_grid->addGarbage(_pendingGarbageCount);
		_pendingGarbageCount = 0;

		renderIncomingGarbage(_x, 40);

		// Switch back to the drop state
		_state = GRID_RUNNER_STATE_DROP;
	} else {

		// Nothing exploded, so we can put a new live block into
		// the grid
		if (!_grid->addLiveBlocks(_nextBlocks[0], _nextBlocks[1])) {

			// Cannot add more blocks - game is over
			_state = GRID_RUNNER_STATE_DEAD;
		} else {

			// Fetch the next blocks from the block server and remember
			// them
			_blockServer->getNextBlocks(_playerNumber, &_nextBlocks[0], &_nextBlocks[1]);

			renderNextBlocks(_x + ((Grid::GRID_WIDTH - 2) * Grid::BLOCK_SIZE / 2), 0);

			_scoreMultiplier = 0;

			// Queue up outgoing blocks for the other player
			if (_outgoingGarbageCount > 0) {
				_pendingGarbageCount -= _outgoingGarbageCount;
				_outgoingGarbageCount = 0;
			}

			_state = GRID_RUNNER_STATE_LIVE;
		}
	}
}

void GridRunner::live() {

	// Player-controllable blocks are in the grid

	if (_grid->hasLiveBlocks()) {

		// Work out how many frames we need to wait until the blocks drop
		// automatically
		s32 timeToDrop = LEVEL_SPEEDS[_level < 20 ? _level : 19];

		// Process user input
		if (_controller->left()) {
			_grid->moveLiveBlocksLeft();
		} else if (_controller->right()) {
			_grid->moveLiveBlocksRight();
		} else if (_controller->down() && (_timer % 2 == 0)) {

			// Force blocks to drop
			_timer = timeToDrop;
		} else if (_controller->rotateClockwise()) {
			_grid->rotateLiveBlocksClockwise();
		} else if (_controller->rotateAntiClockwise()) {
			_grid->rotateLiveBlocksAntiClockwise();
		}

		// Drop live blocks if the timer has expired
		if (_timer >= timeToDrop) {
			_timer = 0;
			_grid->dropLiveBlocks();
		}
	} else {

		// At least one of the blocks in the live pair has touched down.
		// We need to drop the other block automatically
		_state = GRID_RUNNER_STATE_DROP;
	}
}

void GridRunner::iterate() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	// Returns true if any blocks have an animation still in progress
	bool animated = _grid->animate();
	_grid->render(_x, 0, gfx);

	++_timer;

	renderOutgoingGarbage(_x, 48);

	switch (_state) {
		case GRID_RUNNER_STATE_DROP:
			drop();
			break;
		
		case GRID_RUNNER_STATE_LANDING:
			
			// Wait until animations stop
			if (!animated) {
				land();
			}

			break;

		case GRID_RUNNER_STATE_EXPLODING:

			// Wait until animations stop
			if (!animated) {

				// All animations have finished - we need to drop any blocks
				// that are now sat on holes in the grid
				_state = GRID_RUNNER_STATE_DROP;
			}

			break;

		case GRID_RUNNER_STATE_LIVE:
			live();
			break;	

		case GRID_RUNNER_STATE_DEAD:
			
			// TODO: Show loser message here
			break;
		
		case GRID_RUNNER_STATE_WON:

			// TODO: Show winner message here
			break;
	}
}

s32 GridRunner::getOutgoingGarbageCount() const {
	if (_pendingGarbageCount > 0) return 0;
	return 0 - _pendingGarbageCount;
}

s32 GridRunner::getIncomingGarbageCount() const {
	if (_pendingGarbageCount < 0) return 0;
	return _pendingGarbageCount;
}

const Grid* GridRunner::getGrid() const {
	return _grid;
}

bool GridRunner::addIncomingGarbage(s32 count) {
	if (!canReceiveGarbage()) return false;
	if (count < 1) return false;

	_pendingGarbageCount += count;

	renderIncomingGarbage(_x, 40);

	return true;
}

void GridRunner::clearOutgoingGarbageCount() {
	if (_pendingGarbageCount < 0) _pendingGarbageCount = 0;
}

bool GridRunner::canReceiveGarbage() const {
	return _state == GRID_RUNNER_STATE_LIVE;
}

bool GridRunner::isDead() const {
	return _state == GRID_RUNNER_STATE_DEAD;
}

s32 GridRunner::getScore() const {
	return _score;
}

s32 GridRunner::getChains() const {
	return _chains;
}

s32 GridRunner::getLevel() const {
	return _level;
}
