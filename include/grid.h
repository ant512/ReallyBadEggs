#ifndef _GRID_H_
#define _GRID_H_

#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>

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

	u8 getBlockAt(s32 x, s32 y) const;
	void setBlockAt(s32 x, s32 y, u8 block);
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

private:

	enum BlockType {
		BLOCK_NONE = 0,
		BLOCK_RED = 1,
		BLOCK_GREEN = 2,
		BLOCK_BLUE = 3,
		BLOCK_GREY = 4
	};

	u8* _data;
	Point* _liveBlocks;
	bool _hasLiveBlocks;
	bool* _dirtyBlocks;

	void setLiveBlocks(u8 block1, u8 block2);
};

#endif
