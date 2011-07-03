#include <woopsistring.h>

#include "gridrunner.h"
#include "hardware.h"
#include "soundplayer.h"

const s32 GridRunner::SPEEDS[SPEED_COUNT] = { 38, 34, 30, 26, 22, 18, 14, 10, 6, 2 };

GridRunner::GridRunner(ControllerBase* controller,
					   Grid* grid,
					   BlockServer* blockServer,
					   s32 playerNumber,
					   s32 x,
					   GameType gameType,
					   s32 speed) {

	_state = GRID_RUNNER_STATE_DROP;
	_timer = 0;
	_controller = controller;
	_grid = grid;
	_blockServer = blockServer;
	_playerNumber = playerNumber;
	_x = x;
	_gameType = gameType;

	_score = 0;
	_speed = speed;
	_chains = 0;
	_scoreMultiplier = 0;
	_outgoingGarbageCount = 0;
	_incomingGarbageCount = 0;
	_accumulatingGarbageCount = 0;

	_droppingLiveBlocks = false;

	_nextBlocks = new BlockBase*[2];

	// Ensure we have some initial blocks to add to the grid
	_blockServer->getNextBlocks(_playerNumber, &_nextBlocks[0], &_nextBlocks[1]);
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

void GridRunner::renderHUD() {
	renderScore(_x + (Grid::BLOCK_SIZE / 2), (Grid::BLOCK_SIZE * 2) + (Grid::BLOCK_SIZE / 2));
	renderSpeed(_x + (Grid::BLOCK_SIZE / 2), (Grid::BLOCK_SIZE * 4) + (Grid::BLOCK_SIZE / 2));
	renderChainCount(_x + (Grid::BLOCK_SIZE / 2), (Grid::BLOCK_SIZE * 6) + (Grid::BLOCK_SIZE / 2));
	renderIncomingGarbage();
}

void GridRunner::renderScore(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str("Score");
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));

	y += _font.getHeight();

	str.format("%06d", _score);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderSpeed(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str("Speed");
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));

	y += _font.getHeight();

	str.format("%d", _speed);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderChainCount(s32 x, s32 y) {
	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	WoopsiGfx::WoopsiString str("Chains");
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));

	y += _font.getHeight();

	str.format("%04d", _chains);

	gfx->drawFilledRect(x, y, _font.getStringWidth(str), _font.getHeight(), woopsiRGB(0, 0, 0));
	gfx->drawText(x, y, &_font, str, 0, str.getLength(), woopsiRGB(31, 31, 31));
}

void GridRunner::renderIncomingGarbage() {

	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	s32 garbage = _incomingGarbageCount;
	
	s32 faceBoulders = garbage / GARBAGE_FACE_BOULDER_VALUE;
	garbage -= faceBoulders * GARBAGE_FACE_BOULDER_VALUE;

	s32 largeBoulders = _incomingGarbageCount / GARBAGE_LARGE_BOULDER_VALUE;
	garbage -= largeBoulders * GARBAGE_LARGE_BOULDER_VALUE;

	s32 bmpY = Grid::BLOCK_SIZE;
	s32 boulderX = _x + (Grid::BLOCK_SIZE * 5);

	// Erase old boulders
	gfx->drawFilledRect(boulderX, Grid::BLOCK_SIZE, Grid::BLOCK_SIZE, 8 * Grid::BLOCK_SIZE, woopsiRGB(0, 0, 0));
	
	// Draw face boulders
	for (s32 i = 0; i < faceBoulders; ++i) {
		gfx->drawBitmap(boulderX, bmpY, _faceBoulderBmp.getWidth(), _faceBoulderBmp.getHeight(), &_faceBoulderBmp, 0, 0);

		bmpY += _faceBoulderBmp.getHeight();
	}

	// Draw large boulders
	for (s32 i = 0; i < largeBoulders; ++i) {
		gfx->drawBitmap(boulderX, bmpY, _largeBoulderBmp.getWidth(), _largeBoulderBmp.getHeight(), &_largeBoulderBmp, 0, 0);

		bmpY += _largeBoulderBmp.getHeight();
	}

	// Draw small boulders
	for (s32 i = 0; i < garbage; ++i) {
		gfx->drawBitmap(boulderX, bmpY, _smallBoulderBmp.getWidth(), _smallBoulderBmp.getHeight(), &_smallBoulderBmp, 0, 0);

		bmpY += _smallBoulderBmp.getHeight();
	}
}

void GridRunner::renderNextBlocks() const {

	WoopsiGfx::Graphics* gfx = Hardware::getBottomGfx();

	s32 renderX = _x + (((Grid::GRID_WIDTH * Grid::BLOCK_SIZE) - (Grid::BLOCK_SIZE * 2))/ 2);

	for (s32 i = 0; i < 2; ++i) {
		BlockBase* block = _nextBlocks[i];

		if (block == NULL) {
			gfx->drawFilledRect(renderX, Grid::BLOCK_SIZE, Grid::BLOCK_SIZE, Grid::BLOCK_SIZE, woopsiRGB(0, 0, 0));
		} else {
			block->render(renderX, Grid::BLOCK_SIZE, gfx);
		}

		renderX += Grid::BLOCK_SIZE;
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

		SoundPlayer::playChain(_playerNumber, _scoreMultiplier);
		
		++_scoreMultiplier;
		_score += score * _scoreMultiplier;

		_chains += chains;

		if (_gameType == GAME_TYPE_TWO_PLAYER) {
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

			_accumulatingGarbageCount += garbage;
		}

		renderScore(_x + (Grid::BLOCK_SIZE / 2), (Grid::BLOCK_SIZE * 2) + (Grid::BLOCK_SIZE / 2));
		renderSpeed(_x + (Grid::BLOCK_SIZE / 2), (Grid::BLOCK_SIZE * 4) + (Grid::BLOCK_SIZE / 2));
		renderChainCount(_x + (Grid::BLOCK_SIZE / 2), (Grid::BLOCK_SIZE * 6) + (Grid::BLOCK_SIZE / 2));

		// We need to run the explosion animations next
		_state = GRID_RUNNER_STATE_EXPLODING;

	} else if (_incomingGarbageCount > 0) {

		// Add any incoming garbage blocks
		_grid->addGarbage(_incomingGarbageCount);

		// Switch back to the drop state
		_state = GRID_RUNNER_STATE_DROP;

		_incomingGarbageCount = 0;
		renderIncomingGarbage();
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

			renderNextBlocks();

			if (_scoreMultiplier > 1) SoundPlayer::playMultichain(_playerNumber);

			_scoreMultiplier = 0;

			// Queue up outgoing blocks for the other player
			_outgoingGarbageCount += _accumulatingGarbageCount;
			_accumulatingGarbageCount = 0;

			_state = GRID_RUNNER_STATE_LIVE;
		}
	}
}

void GridRunner::live() {

	// Player-controllable blocks are in the grid

	if (_grid->hasLiveBlocks()) {

		// Work out how many frames we need to wait until the blocks drop
		// automatically
		s32 timeToDrop = SPEEDS[_speed];

		// Process user input
		if (_controller->left()) {
			_grid->moveLiveBlocksLeft();
		} else if (_controller->right()) {
			_grid->moveLiveBlocksRight();
		}

		if (_controller->down() && (_timer % 2 == 0)) {

			// Force blocks to drop
			_timer = timeToDrop;

			if (!_droppingLiveBlocks) {
				_droppingLiveBlocks = true;

				SoundPlayer::playDrop(_playerNumber);
			}
		} else if (!_controller->down()) {
			_droppingLiveBlocks = false;
		}
		
		if (_controller->rotateClockwise()) {
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
		_droppingLiveBlocks = false;
		_state = GRID_RUNNER_STATE_DROP;
	}
}

void GridRunner::iterate() {

	WoopsiGfx::Graphics* gfx = Hardware::getTopGfx();

	// Returns true if any blocks have an animation still in progress
	bool animated = _grid->animate();
	_grid->render(_x, 0, gfx);

	++_timer;

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
			break;
	}
}

s32 GridRunner::getOutgoingGarbageCount() const {
	return _outgoingGarbageCount;
}

s32 GridRunner::getIncomingGarbageCount() const {
	return _incomingGarbageCount;
}

const Grid* GridRunner::getGrid() const {
	return _grid;
}

bool GridRunner::addIncomingGarbage(s32 count) {
	if (!canReceiveGarbage()) return false;
	if (count < 1) return false;

	_incomingGarbageCount += count;

	renderIncomingGarbage();

	return true;
}

void GridRunner::clearOutgoingGarbageCount() {
	_outgoingGarbageCount = 0;
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

s32 GridRunner::getSpeed() const {
	return _speed;
}
