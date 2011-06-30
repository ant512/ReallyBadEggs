#include "aicontroller.h"

#include "hardware.h"
#include "gamefont.h"
#include "woopsistring.h"

// TODO: Doesn't check if it can rotate/move to the desired co-ordinate from
// the current position.  This should probably be re-examined every time the AI
// has the chance to move.

AIController::AIController(s32 hesitation) {
	_gridRunner = NULL;
	_lastLiveBlockY = Grid::GRID_HEIGHT;
	_targetX = 0;
	_targetRotations = 0;
	_hesitation = hesitation;
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

	// Work out which columns have heights equal to or greater than the current
	// live block Y co-ordinates and constrain the search to within the
	// boundaries that they create
	s32 leftBoundary = -1;
	s32 rightBoundary = Grid::GRID_WIDTH;
	s32 lowestYCoord = liveBlock1.y > liveBlock2.y ? liveBlock1.y : liveBlock2.y;
	s32 leftBlockXCoord = liveBlock1.x < liveBlock2.x ? liveBlock1.x : liveBlock2.x;
	s32 rightBlockXCoord = liveBlock1.x > liveBlock2.x ? liveBlock1.x : liveBlock2.x;

	for (s32 i = leftBlockXCoord; i >= 0; --i) {
		if (columnYCoords[i] <= lowestYCoord) {
			leftBoundary = i;
			break;
		}
	}

	for (s32 i = rightBlockXCoord; i < Grid::GRID_WIDTH; ++i) {
		if (columnYCoords[i] <= lowestYCoord) {
			rightBoundary = i;
			break;
		}
	}

	BlockBase* block1 = grid->getBlockAt(liveBlock1.x, liveBlock1.y);
	BlockBase* block2 = grid->getBlockAt(liveBlock2.x, liveBlock2.y);

	s32 bestScore = 0;

	Point point1;
	Point point2;

	for (s32 x = leftBoundary + 1; x < rightBoundary; ++x) {
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
					point1.x = x;
					point1.y = columnYCoords[x] - 1;

					point2.x = x;
					point2.y = columnYCoords[x];
					break;
				
				case 2:
					point1.x = x + 1;
					point1.y = columnYCoords[x + 1];

					point2.x = x;
					point2.y = columnYCoords[x];
					break;

				case 3:
					point1.x = x;
					point1.y = columnYCoords[x];

					point2.x = x;
					point2.y = columnYCoords[x] - 1;
					break;
			}

			// Check if the new co-ords are valid
			if (point1.x < 0 || point1.x >= Grid::GRID_WIDTH) break;
			if (point1.y < 0 || point1.y >= Grid::GRID_HEIGHT) break;
			if (point2.x < 0 || point2.x >= Grid::GRID_WIDTH) break;
			if (point2.y < 0 || point2.y >= Grid::GRID_HEIGHT) break;

			s32 score = scoreShapePosition(block1, block2, point1, point2);

			// Bonus for not increasing the height of the target column
			s32 heightBonus = 1 + ((point1.y + point2.y));

			score *= heightBonus;

			// Check if the score for this position and rotation beats the
			// current best
			if (score > bestScore) {
				bestScore = score;
				_targetX = point1.x < point2.x ? point1.x : point2.x;
				_targetRotations = rotation;
			}
		}
	}

	// We need to determine if the shape has already been rotated and adjust
	// accordingly
	if (liveBlock1.x == liveBlock2.x) {
		if (liveBlock1.y == liveBlock2.y - 1) {

			// Block 1 is above block 2, therefore exising rotation is 1
			--_targetRotations;
		} else {

			// Block 1 is below block 2, therefore existing rotation is 3
			_targetRotations -= 3;
		}
	} else if (liveBlock1.x == liveBlock2.x + 1) {

		// Block 1 is on the right of block 2, therefore existing rotation is 2
		_targetRotations -= 2;
	}

	// We can rotate to the correct orientation faster by rotating anticlockwise
	// if necessary
	if (_targetRotations == 3) _targetRotations = -1;

	delete[] columnYCoords;
}

s32 AIController::scoreShapePosition(BlockBase* block1, BlockBase* block2, const Point& point1, const Point& point2) {

	const Grid* grid = _gridRunner->getGrid();

	s32 gridSize = Grid::GRID_WIDTH * Grid::GRID_HEIGHT;

	bool* checkedData = new bool[gridSize];

	for (s32 i = 0; i < gridSize; ++i) {
		checkedData[i] = false;
	}

	// Unfortunately, we can't get the score for each possible single block
	// position, then add together pairs and see what the total score would be
	// for each possible rotation (this would have the number of times we walk
	// the grid graph).  If blocks are the same colour, and we do not examine
	// the same checkedData array whilst checking for chain lengths, we may end
	// up walking over the same blocks twice.  They will therefore be included
	// in the score multiple times (once for each block and once when the scores
	// are added together).  This would lead to positions where both blocks
	// touched same colour blocks being massively overweighted and possibly
	// supplant better positions.
	s32 score1 = grid->getPotentialChainLength(point1.x, point1.y, block1, checkedData);
	s32 score2 = grid->getPotentialChainLength(point2.x, point2.y, block2, checkedData);

	delete[] checkedData;

	s32 score = 0;
	
	if ((block1->getColour() == block2->getColour()) && ((point1.x == point2.x) || (point1.y == point2.y))) {
		score = 1 << (score1 + score2);
	} else {
		score = 1 << score1;
		score += 1 << score2;
	}
	
	return score;
}

bool AIController::canMoveToTarget() {

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);

	// Get the y co-ords of the topmost blank block in each column
	s32* columnYCoords = new s32[Grid::GRID_WIDTH];

	for (s32 i = 0; i < Grid::GRID_WIDTH; ++i) {
		columnYCoords[i] = (Grid::GRID_HEIGHT - grid->getColumnHeight(i)) - 1;
	}

	bool movePossible = true;

	if (_targetX < liveBlock1.x) {

		// Need to move left
		for (s32 x = liveBlock1.x; x >= _targetX; --x) {

			// If either block is lower than the column we're looking at, it is
			// impossible for the block to be moved to the target column
			if (columnYCoords[x] < liveBlock1.y || columnYCoords[x] < liveBlock2.y) {
				movePossible = false;
			}
		}
	} else if (_targetX > liveBlock1.x) {

		// Need to move right
		for (s32 x = liveBlock1.x; x <= _targetX; ++x) {
			if (columnYCoords[x] < liveBlock1.y || columnYCoords[x] < liveBlock2.y) {
				movePossible = false;
			}
		}
	}

	return movePossible;
}

bool AIController::left() {
	analyseGrid();

	if (_targetRotations != 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x > _targetX;

	return _hesitation == 0 ? result : result && (rand() % _hesitation == 0);
}

bool AIController::right() {
	analyseGrid();

	if (_targetRotations != 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x < _targetX;

	return _hesitation == 0 ? result : result && (rand() % _hesitation == 0);
}

bool AIController::down() {
	analyseGrid();

	if (_targetRotations != 0) return false;

	const Grid* grid = _gridRunner->getGrid();

	Point liveBlock1;
	Point liveBlock2;

	grid->getLiveBlockPoints(liveBlock1, liveBlock2);
	
	bool result = liveBlock1.x == _targetX;

	return _hesitation == 0 ? result : result && (rand() % _hesitation == 0);
}

bool AIController::rotateClockwise() {
	analyseGrid();

	if (_targetRotations > 0) {
		--_targetRotations;

		return true;
	}

	return false;
}

bool AIController::rotateAntiClockwise() {
	analyseGrid();

	if (_targetRotations < 0) {
		++_targetRotations;

		return true;
	}

	return false;
}
