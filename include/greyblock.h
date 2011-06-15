#ifndef _GREY_BLOCK_H_
#define _GREY_BLOCK_H_

#include <animation.h>
#include <bitmapbase.h>
#include <graphics.h>
#include <nds.h>

#include "blockbase.h"

class GreyBlock : public BlockBase {
public:
	GreyBlock();
	~GreyBlock();

	void connect(const BlockBase* top, const BlockBase* right, const BlockBase* bottom, const BlockBase* left) { };
};

#endif
