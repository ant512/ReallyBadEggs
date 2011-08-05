#include "normalblock.h"
#include "redblockbmp.h"

NormalBlock::NormalBlock(u16 colour) : BlockBase(colour) {
}

NormalBlock::~NormalBlock() {
}

void NormalBlock::connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) {
	setConnections(top != NULL && top->getColour() == _colour && top->getState() == BlockBase::BLOCK_STATE_NORMAL,
				   right != NULL && right->getColour() == _colour && right->getState() == BlockBase::BLOCK_STATE_NORMAL,
				   bottom != NULL && bottom->getColour() == _colour && bottom->getState() == BlockBase::BLOCK_STATE_NORMAL,
				   left != NULL && left->getColour() == _colour && left->getState() == BlockBase::BLOCK_STATE_NORMAL);
}

