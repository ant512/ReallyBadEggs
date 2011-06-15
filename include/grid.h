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
	Grid(s32 blockColoursCount);

	/**
	 * Destructor.
	 */
	~Grid();

	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx);
	void iterate();
	void clear();

	BlockBase* getBlockAt(s32 x, s32 y) const;
	void setBlockAt(s32 x, s32 y, BlockBase* block);
	void moveBlock(s32 srcX, s32 srcY, s32 destX, s32 destY);
	bool isValidCoordinate(s32 x, s32 y) const;

	void getChain(s32 x, s32 y, WoopsiArray<Point>& chain, bool* checkedData) const;
	void getChains(WoopsiArray<WoopsiArray<Point>*>& chains) const;

	s32 explodeChains();

	void dropLiveBlocks();
	bool dropBlocks();

	void moveLiveBlocksLeft();
	void moveLiveBlocksRight();

	void rotateLiveBlocksClockwise();
	void rotateLiveBlocksAntiClockwise();

	bool hasLiveBlocks() const;

	void addLiveBlocks();

	void connectBlocks();

	bool animate();




	/**
	 * Draws the two next blocks at the specified co-ordinates.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 * @param gfx The graphics object to draw with.
	 */
	void renderNextBlocks(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

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

	BlockBase** _data;			/**< Grid of blocks. */
	BlockBase** _nextBlocks;	/**< Array of 2 blocks that will be placed next. */
	Point* _liveBlocks;			/**< Array of 2 Point objects that track the location of the live blocks. */
	bool _hasLiveBlocks;		/**< True if there are any live blocks in the grid. */
	s32 _blockColourCount;		/**< Number of block colours available. */

	/**
	 * Creates a new randomly-coloured block.  Should be deleted when no longer
	 * needed.
	 */
	BlockBase* newRandomBlock() const;

	/**
	 * Populates the next blocks array with two new blocks.  Deletes any
	 * existing blocks in the array so it is safe to call it repeatedly without
	 * leaking memory.
	 */
	void chooseNextBlocks();
};

#endif
