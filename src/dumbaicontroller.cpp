#include "dumbaicontroller.h"

DumbAIController::DumbAIController(bool isFast) {
	_gridRunner = NULL;
	_lastLiveBlockY = Grid::GRID_HEIGHT;
	_targetX = 0;
	_targetRotations = 0;
	_isFast = isFast;
}

void DumbAIController::analyseGrid() {

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);

	// If last observed y is greater than current live block y, we'll need
	// to choose a new move
	if (_lastLiveBlockY > liveBlock1.y) {

		// Get the y co-ords of the topmost blank block in each column
		s32 columnY[Grid::GRID_WIDTH];

		for (s32 i = 0; i < Grid::GRID_WIDTH; ++i) {
			columnY[i] = (Grid::GRID_HEIGHT - grid->getColumnHeight(i)) - 1;
		}

		s32 bestScore = 0;
		s32 bestScoreX = 0;
		bool rotate = false;
		s32 chainLength[2];

		bool* checkedData = new bool[Grid::GRID_WIDTH * Grid::GRID_HEIGHT];

		// Get the score for each possible move we can make.  Assumptions:
		// - Blocks are only ever placed horizontally;
		// - There is nothing to stop us moving a block (ie. no filled
		//   columns).
		for (s32 arrangement = 0; arrangement < 2; ++arrangement) {

			for (s32 i = 0; i < Grid::GRID_WIDTH - 1; ++i) {

				// Wipe the checked data
				for (s32 j = 0; j < Grid::GRID_WIDTH * Grid::GRID_HEIGHT; ++j) {
					checkedData[j] = false;
				}

				chainLength[0] = 0;
				chainLength[1] = 0;

				// Don't try to add blocks to columns that are already as tall
				// as the grid
				if (columnY[i] >= 0) {
					chainLength[0] = grid->getPotentialChainLength(i, columnY[i], grid->getBlockAt(liveBlock1.x, liveBlock1.y), checkedData);
				}

				if (columnY[i + 1] >= 0) {
					chainLength[1] = grid->getPotentialChainLength(i + 1, columnY[i + 1], grid->getBlockAt(liveBlock2.x, liveBlock2.y), checkedData);
				}

				// Basic score is the number of blocks connected together,
				// regardless of whether explosions will be created
				s32 chainBaseScore = chainLength[0] + chainLength[1];

				// Extra score gives a bonus for creating exploding chains
				s32 chainExtraScore = chainLength[0] > Grid::CHAIN_LENGTH ? 1 + chainLength[0] - Grid::CHAIN_LENGTH : 1;

				// Join the chains together if the blocks are the same colour
				// and adjacent
				if (grid->getBlockAt(liveBlock1.x, liveBlock1.y)->getColour() == grid->getBlockAt(liveBlock2.x, liveBlock2.y)->getColour() &&
					columnY[i] == columnY[i + 1]) {

					chainExtraScore += chainLength[1];
				} else {
					chainExtraScore += chainLength[1] > Grid::CHAIN_LENGTH ? 1 + chainLength[1] - Grid::CHAIN_LENGTH : 1;
				}

				// Bonus for not increasing the height of the target column
				s32 heightBonus = 1 + ((columnY[i] + columnY[i + 1]) / 2);

				s32 score = chainBaseScore * chainExtraScore * heightBonus;

				if (score > bestScore) {
					bestScore = score;
					bestScoreX = i;

					rotate = arrangement == 1;
				}
			}

			// If both blocks are the same, we don't need to try the
			// alternative rotation
			if (grid->getBlockAt(liveBlock1.x, liveBlock1.y)->getColour() == grid->getBlockAt(liveBlock2.x, liveBlock2.y)->getColour()) break;
			
			// Switch the blocks and try again
			grid->getLiveBlockPoints(liveBlock2, liveBlock1);
		}

		delete[] checkedData;

		// Get a new target column
		_targetX = bestScoreX;

		if (rotate) _targetRotations = 2;
	}

	// Remember the co-ord of the top block in the grid so that next time we
	// can identify if we've got a new live block or not
	_lastLiveBlockY = liveBlock1.y < liveBlock2.y ? liveBlock1.y : liveBlock2.y;
}

void DumbAIController::setGridRunner(const GridRunner* gridRunner) {
	_gridRunner = gridRunner;
}

bool DumbAIController::left() {
	analyseGrid();

	if (_targetRotations > 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x > _targetX;

	return _isFast ? result : result && (rand() % SLOWDOWN_CHANCE == 0);
}

bool DumbAIController::right() {
	analyseGrid();

	if (_targetRotations > 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x < _targetX;

	return _isFast ? result : result && (rand() % SLOWDOWN_CHANCE == 0);
}

bool DumbAIController::down() {
	analyseGrid();

	if (_targetRotations > 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x == _targetX;

	return _isFast ? result : result && (rand() % SLOWDOWN_CHANCE == 0);
}

bool DumbAIController::rotateClockwise() {
	analyseGrid();

	if (_targetRotations > 0) {
		--_targetRotations;

		return _isFast ? true : rand() % SLOWDOWN_CHANCE == 0;
	}

	return false;
}

bool DumbAIController::rotateAntiClockwise() {
	return false;
}
