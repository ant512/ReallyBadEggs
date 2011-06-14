#include "normalblock.h"
#include "redblockbmp.h"

NormalBlock::NormalBlock(u16 colour) : BlockBase(colour) {
}

NormalBlock::~NormalBlock() {
}

void NormalBlock::connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) {
	setConnections(top != NULL && top->getColour() == _colour && top->isConnectable(),
				   right != NULL && right->getColour() == _colour && right->isConnectable(),
				   bottom != NULL && bottom->getColour() == _colour && bottom->isConnectable(),
				   left != NULL && left->getColour() == _colour && left->isConnectable());
}

