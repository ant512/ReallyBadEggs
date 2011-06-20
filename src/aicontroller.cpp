#include "aicontroller.h"

AIController::AIController() {
	_gridRunner = NULL;
	_lastLiveBlockY = Grid::GRID_HEIGHT;
	_targetX = 0;
}

void AIController::analyseGrid() {

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

		// Get the score for each possible move we can make.  Assumptions:
		// - We never rotate a block;
		// - There is nothing to stop us moving a block (ie. no filled
		//   columns).
		for (s32 i = 0; i <  Grid::GRID_WIDTH - 1; ++i) {

			s32 chainLength[2];
			chainLength[0] = 0;
			chainLength[1] = 0;

			// Don't try to add blocks to columns that are already as tall
			// as the grid
			if (columnY[i] >= 0) {
				chainLength[0] = grid->getPotentialChainLength(i, columnY[i], grid->getBlockAt(liveBlock1.x, liveBlock1.y));
			}

			if (columnY[i + 1] >= 0) {
				chainLength[1] = grid->getPotentialChainLength(i, columnY[i + 1], grid->getBlockAt(liveBlock2.x, liveBlock2.y));
			}

			// Basic score is the number of blocks connected together,
			// regardless of whether explosions will be created
			s32 chainBaseScore = (chainLength[0] + chainLength[1]) * 20;

			// Extra score gives a bonus for creating exploding chains
			s32 chainExtraScore = chainLength[0] > Grid::CHAIN_LENGTH ? 1 + chainLength[0] - Grid::CHAIN_LENGTH : 1;

			// Join the chains together if the blocks are the same colour
			if (grid->getBlockAt(liveBlock1.x, liveBlock1.y)->getColour() == grid->getBlockAt(liveBlock2.x, liveBlock2.y)->getColour()) {
				chainExtraScore += chainLength[1];
			} else {
				chainExtraScore += chainLength[1] > Grid::CHAIN_LENGTH ? 1 + chainLength[1] - Grid::CHAIN_LENGTH : 1;
			}

			// Penalise for increasing the height of the target column
			s32 heightPenalty = columnY[i] + columnY[i + 1];

			s32 score = chainBaseScore * chainExtraScore / heightPenalty;

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
}

void AIController::setGridRunner(const GridRunner* gridRunner) {
	_gridRunner = gridRunner;
}

bool AIController::left() {
	analyseGrid();

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	return (liveBlock1.x > _targetX);
}

bool AIController::right() {
	analyseGrid();

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	return (liveBlock1.x < _targetX);
}

bool AIController::down() {
	analyseGrid();

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	return (liveBlock1.x == _targetX);
}

bool AIController::rotateClockwise() {
	return false;
}

bool AIController::rotateAntiClockwise() {
	return false;
}
