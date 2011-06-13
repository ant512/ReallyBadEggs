#include "blockbase.h"

bool BlockBase::connect(BlockBase* block) {
	if (block->getY() == _y) {
		if (block->getX() == _x - 1) {
			block->setRightConnection(true);
			_leftConnection = true;
			return true;
		} else if (block->getX() == _x + 1) {
			block->setLeftConnection(true);
			_rightConnection = true;
			return true;
		}
	}

	return false;
}
