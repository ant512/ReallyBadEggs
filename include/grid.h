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

	/**
	 * Draws the grid to the specified co-ordinats.
	 * @param x The x co-ordinate to draw to.
	 * @param y The y co-ordinate to draw to.
	 * @param gfx The graphics object to draw with.
	 */
	void render(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

	/**
	 * Draws the two next blocks at the specified co-ordinates.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 * @param gfx The graphics object to draw with.
	 */
	void renderNextBlocks(s32 x, s32 y, WoopsiGfx::Graphics* gfx);

	/**
	 * Wipes all data from the grid, including all blocks and the next blocks.
	 */
	void clear();

	/**
	 * Sets all blocks that are part of a chain of minimum length CHAIN_LENGTH
	 * to exploding.
	 * TODO: Fix the output of this.
	 * @return A value representing the number of chains/blocks exploded.
	 */
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
	 * Gets the block at the specified co-ordinates.
	 * @param x The x co-ordinate of the block.
	 * @param y The y co-ordinate of the block.
	 * @return A pointer to the block, or NULL if no block exists.
	 */
	BlockBase* getBlockAt(s32 x, s32 y) const;

	/**
	 * Inserts the supplied block at the specified co-ordinates.  If a block
	 * already exists at those co-ordinates it is deleted.  Note that the block
	 * must not exist at another co-ordinate within the grid or weird things
	 * will happen.
	 * @param x The x co-ordinate of the block.
	 * @param y The y co-ordinate of the block.
	 * @param block The block to add to the grid.
	 */
	void setBlockAt(s32 x, s32 y, BlockBase* block);

	/**
	 * Move a block from one co-ordinate to another.  If a block exists at the
	 * destination co-ordinates it is deleted.  If the source and destination
	 * co-ordinates are the same no action is taken.
	 * @param srcX The x co-ordinate of the source block.
	 * @param srcY The y co-ordinate of the source block.
	 * @param destX The x co-ordinate of the destination block.
	 * @param destY The y co-ordinate of the destination block.
	 */
	void moveBlock(s32 srcX, s32 srcY, s32 destX, s32 destY);

	/**
	 * Check if the supplied co-ordinates fall within the grid.
	 * @param srcX The x co-ordinate to check.
	 * @param srcY The y co-ordinate to check.
	 * @return True if the co-ordinates are within the grid; false if not.
	 */
	bool isValidCoordinate(s32 x, s32 y) const;

	/**
	 * Gets the co-ordinates of all blocks in the chain that starts with the
	 * block at (x,y).
	 * @param x The x co-ordinate of the block.
	 * @param y The y co-ordinate of the block.
	 * @param chain The array into which the co-ordinates of the blocks in the
	 * chain will be placed.  This must be a pre-existing, empty array and will
	 * be populated by the method.
	 * @param checkedData An array of bools that is the same size as the grid
	 * data array.  As each block in the chain is identified, it is marked as
	 * checked in this array.  A value of true indicates that the block at
	 * the same co-ordinate in the data array is part of this chain.  A value of
	 * false indicates that the block is not a part of this chain.  The array
	 * must be a pre-existing array of bools all set to false.  It is used
	 * when identifying multiple chains to prevent against checking the same
	 * blocks over and over again.
	 */
	void getChain(s32 x, s32 y, WoopsiArray<Point>& chain, bool* checkedData) const;

	/**
	 * Gets all chains with length of at least CHAIN_LENGTH that exist in the
	 * grid.
	 * @param chains Each chain is stored as an array of points indicating the
	 * co-ordinates of each block in the data grid.  This value therefore
	 * contains an array of all chain arrays.  It must be a pre-existing, empty
	 * array and will be populated by the method.
	 */
	void getChains(WoopsiArray<WoopsiArray<Point>*>& chains) const;

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
