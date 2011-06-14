#include "normalblock.h"

NormalBlock::NormalBlock(u16 colour) : BlockBase(colour) {

}

NormalBlock::~NormalBlock() {

}

void NormalBlock::connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) {
	_topConnection = top != NULL && top->getColour() == _colour;
	_leftConnection = left != NULL && left->getColour() == _colour;
	_rightConnection = right != NULL && right->getColour() == _colour;
	_bottomConnection = bottom != NULL && bottom->getColour() == _colour;
}

