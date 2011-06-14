#ifndef _NORMAL_BLOCK_H_
#define _NORMAL_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "blockbase.h"

class NormalBlock : public BlockBase {
public:
	NormalBlock(u16 colour);
	~NormalBlock();

	void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left);
};

#endif
