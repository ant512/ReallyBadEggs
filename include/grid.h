#ifndef _GRID_H_
#define _GRID_H_

#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>

#include "blockbase.h"
#include "point.h"

class Grid {
public:

	static const s32 GRID_WIDTH = 6;		/**< Width of the grid. */
	static const s32 GRID_HEIGHT = 12;		/**< Height of the grid. */
	static const s32 CHAIN_LENGTH = 4;		/**< Minimum length for a chain of blocks to be removed. */
	static const s32 BLOCK_SIZE = 16;		/**< Size of each block in the grid. */

	/**
	 * Constructor.
	 */
	Grid();

	/**
	 * Destructor.
	 */
	~Grid();

	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx);
	void renderDirty(s32 x, s32 y, WoopsiGfx::Graphics* gfx);
	void iterate();
	void clear();

	BlockBase* getBlockAt(s32 x, s32 y) const;
	void setBlockAt(s32 x, s32 y, BlockBase* block);
	void moveBlock(s32 srcX, s32 srcY, s32 destX, s32 destY);
	bool isValidCoordinate(s32 x, s32 y) const;

	void getChain(s32 x, s32 y, WoopsiArray<Point>& chain, bool* checkedData) const;
	void getChains(WoopsiArray<WoopsiArray<Point>*>& chains) const;

	s32 removeChains();

	void dropLiveBlocks();
	bool dropBlocks();

	void moveLiveBlocksLeft();
	void moveLiveBlocksRight();

	void rotateLiveBlocksClockwise();
	void rotateLiveBlocksAntiClockwise();

	bool hasLiveBlocks() const;

	void addLiveBlocks();

	void connectBlocks();

private:

	enum BlockType {
		BLOCK_NONE = 0,
		BLOCK_RED = 1,
		BLOCK_BLUE = 2,
		BLOCK_YELLOW = 3,
		BLOCK_PURPLE = 4,
		BLOCK_GREEN = 5,
		BLOCK_ORANGE = 6,
		BLOCK_GREY = 7
	};

	BlockBase** _data;
	Point* _liveBlocks;
	bool _hasLiveBlocks;
	bool* _dirtyBlocks;
	s32 _blockColourCount;

	BlockBase* newRandomBlock() const;
};

#endif
