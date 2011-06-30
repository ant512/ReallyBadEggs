#include "aicontroller.h"

#include "hardware.h"
#include "gamefont.h"
#include "woopsistring.h"

// TODO: Doesn't check if it can rotate/move to the desired co-ordinate from
// the current position.  This should probably be re-examined every time the AI
// has the chance to move.

AIController::AIController(bool isFast) {
	_gridRunner = NULL;
	_lastLiveBlockY = Grid::GRID_HEIGHT;
	_targetX = 0;
	_targetRotations = 0;
	_isFast = isFast;
}

void AIController::setGridRunner(const GridRunner* gridRunner) {
	_gridRunner = gridRunner;
}

void AIController::analyseGrid() {

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);

	// If last observed y is greater than current live block y, we'll need
	// to choose a new move
	if (_lastLiveBlockY <= liveBlock1.y) {
		_lastLiveBlockY = liveBlock1.y < liveBlock2.y ? liveBlock1.y : liveBlock2.y;
		return;
	}
	
	_lastLiveBlockY = liveBlock1.y < liveBlock2.y ? liveBlock1.y : liveBlock2.y;

	// Get the y co-ords of the topmost blank block in each column
	s32* columnYCoords = new s32[Grid::GRID_WIDTH];

	for (s32 i = 0; i < Grid::GRID_WIDTH; ++i) {
		columnYCoords[i] = (Grid::GRID_HEIGHT - grid->getColumnHeight(i)) - 1;
	}

	BlockBase* block1 = grid->getBlockAt(liveBlock1.x, liveBlock1.y);
	BlockBase* block2 = grid->getBlockAt(liveBlock2.x, liveBlock2.y);

	s32 bestScore = 0;

	Point point1;
	Point point2;

	for (s32 x = 0; x < Grid::GRID_WIDTH - 1; ++x) {
		for (s32 rotation = 0; rotation < 4; ++rotation) {

			// Work out where the shapes will be if they move, rotation occurs
			// and they land
			switch (rotation) {
				case 0:
					point1.x = x;
					point1.y = columnYCoords[x];

					point2.x = x + 1;
					point2.y = columnYCoords[x + 1];
					break;
				
				case 1:
					point1.x = x + 1;
					point1.y = columnYCoords[x + 1] - 1;

					point2.x = x + 1;
					point2.y = columnYCoords[x + 1];
					break;
				
				case 2:
					point1.x = x + 1;
					point1.y = columnYCoords[x + 1];

					point2.x = x;
					point2.y = columnYCoords[x];
					break;

				case 3:
					point1.x = x + 1;
					point1.y = columnYCoords[x + 1];

					point2.x = x + 1;
					point2.y = columnYCoords[x + 1] - 1;
					break;
			}

			// Check if the new co-ords are valid
			if (point1.x < 0 || point1.x >= Grid::GRID_WIDTH) break;
			if (point1.y < 0 || point1.y >= Grid::GRID_HEIGHT) break;
			if (point2.x < 0 || point2.x >= Grid::GRID_WIDTH) break;
			if (point2.y < 0 || point2.y >= Grid::GRID_HEIGHT) break;

			s32 score = scoreShapePosition(block1, block2, point1, point2);

			// Bonus for not increasing the height of the target column
			s32 heightBonus = 1 + ((point1.y + point2.y) / 2);

			score *= heightBonus;

			// Check if the score for this position and rotation beats the
			// current best
			if (score > bestScore) {
				bestScore = score;
				_targetX = x;
				_targetRotations = rotation;
			}
		}
	}

	// We can rotate to the correct orientation faster by rotating anticlockwise
	// if necessary
	if (_targetRotations == 3) _targetRotations = -1;

	delete columnYCoords;
}

s32 AIController::scoreShapePosition(BlockBase* block1, BlockBase* block2, const Point& point1, const Point& point2) {

	const Grid* grid = _gridRunner->getGrid();

	s32 gridSize = Grid::GRID_WIDTH * Grid::GRID_HEIGHT;

	bool* checkedData = new bool[gridSize];

	for (s32 i = 0; i < gridSize; ++i) {
		checkedData[i] = false;
	}

	// Unfortunately, we can't get the score for each possible block position,
	// then add together pairs and see what the total score would be for each
	// possible rotation (this would have the number of times we walk the grid
	// graph).  If blocks are the same colour, and we do not examine the same
	// checkedData array whilst checking for chain lengths, we may end up
	// walking over the same blocks twice.  They will therefore be included in
	// the score multiple times (once for each block and once when the scores
	// are added together).  This would lead to positions where both blocks
	// touched same colour blocks being massively overweighted and possibly
	// supplant better positions.
	s32 score1 = grid->getPotentialChainLength(point1.x, point1.y, block1, checkedData);
	s32 score2 = grid->getPotentialChainLength(point2.x, point2.y, block2, checkedData);

	delete[] checkedData;

	s32 baseScore = score1 + score2;
	s32 extraScore = 0;

	if ((point1.x == point2.x || point1.y == point2.y) && (block1->getColour() == block2->getColour())) {
		extraScore = score1 + score2 - Grid::CHAIN_LENGTH;
	} else {
		extraScore = score1 > Grid::CHAIN_LENGTH ? 1 + score1 - Grid::CHAIN_LENGTH : 1;
		extraScore += score2 > Grid::CHAIN_LENGTH ? 1 + score2 - Grid::CHAIN_LENGTH : 1;
	}

	return baseScore * extraScore;
}

bool AIController::left() {
	analyseGrid();

	if (_targetRotations != 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x > _targetX;

	return _isFast ? result : result && (rand() % SLOWDOWN_CHANCE == 0);
}

bool AIController::right() {
	analyseGrid();

	if (_targetRotations != 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x < _targetX;

	return _isFast ? result : result && (rand() % SLOWDOWN_CHANCE == 0);
}

bool AIController::down() {
	analyseGrid();

	if (_targetRotations != 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x == _targetX;

	return _isFast ? result : result && (rand() % SLOWDOWN_CHANCE == 0);
}

bool AIController::rotateClockwise() {
	analyseGrid();

	if (_targetRotations > 0) {
		--_targetRotations;

		return _isFast ? true : rand() % SLOWDOWN_CHANCE == 0;
	}

	return false;
}

bool AIController::rotateAntiClockwise() {
	analyseGrid();

	if (_targetRotations < 0) {
		++_targetRotations;

		return _isFast ? true : rand() % SLOWDOWN_CHANCE == 0;
	}

	return false;
}
