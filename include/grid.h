#ifndef _GRID_H_
#define _GRID_H_

#include <graphics.h>
#include <nds.h>
#include <woopsiarray.h>

#include "blockbase.h"
#include "point.h"

/**
 * The playing field.  Controls the behaviour of the blocks on screen.
 */
class Grid {
public:

	static const s32 GRID_WIDTH = 6;			/**< Width of the grid. */
	static const s32 GRID_HEIGHT = 12;			/**< Height of the grid. */
	static const s32 CHAIN_LENGTH = 4;			/**< Minimum length for a chain of blocks to be removed. */
	static const s32 BLOCK_SIZE = 16;			/**< Size of each block in the grid. */

	/**
	 * Constructor.
	 * @param startingHeight Number of rows that are filled with grey blocks at
	 * the start of the game.
	 */
	Grid(s32 startingHeight);

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
	 * Wipes all data from the grid, including all blocks and the next blocks.
	 */
	void clear();

	/**
	 * Gets the block at the specified co-ordinates.
	 * @param x The x co-ordinate of the block.
	 * @param y The y co-ordinate of the block.
	 * @return A pointer to the block, or NULL if no block exists.
	 */
	BlockBase* getBlockAt(s32 x, s32 y) const;

	/**
	 * Sets all blocks that are part of a chain of minimum length CHAIN_LENGTH
	 * to exploding.
	 * @param score The score earned by the exploding blocks.  This will be
	 * populated by the method.
	 * @param chainCount The number of chains (joined sets of blocks) exploded.
	 * This will be populated by the method.
	 * @param blocks The blocks exploded.  This will be populated by the method.
	 * @return True if any blocks are exploded.
	 */
	bool explodeChains(s32& score, s32& chainCount, s32 &blocks);

	/**
	 * Drops the live blocks down one row if possible.
	 */
	void dropLiveBlocks();

	/**
	 * Drops all blocks down one row if possible.
	 * @return True if blocks were dropped; false if not.
	 */
	bool dropBlocks();

	/**
	 * Moves the live blocks one column left if possible.
	 */
	void moveLiveBlocksLeft();

	/**
	 * Moves the live blocks on column right if possible.
	 */
	void moveLiveBlocksRight();

	/**
	 * Rotates the live blocks clockwise if possible.
	 */
	void rotateLiveBlocksClockwise();

	/**
	 * Rotates the live blocks anticlockwise if possible.
	 */
	void rotateLiveBlocksAntiClockwise();

	/**
	 * Check if the grid has any live blocks.
	 * @return True if the grid contains live blocks; false if not.
	 */
	bool hasLiveBlocks() const;

	/**
	 * Adds the next live blocks into the grid.
	 */
	bool addLiveBlocks(BlockBase* block1, BlockBase* block2);

	/**
	 * Establishes connections between blocks.
	 */
	void connectBlocks();

	/**
	 * Animates all blocks in the grid.
	 * @return True if any blocks animate; false if not.
	 */
	bool animate();

	/**
	 * Get the height of the specified column.
	 * @param column The column to get the height of.
	 * @return The height of the column.
	 */
	s32 getColumnHeight(s32 column) const;

	/**
	 * Adds the specified amount of garbage blocks to the grid.  Blocks are
	 * placed in the lowest columns first.  If the grid is full, no more blocks
	 * are added.  If the number of blocks exceeds the amount of free space in
	 * the grid, the grid is filled and the function exits.
	 * @param count The number of blocks to place.
	 */
	void addGarbage(s32 count);

	/**
	 * Gets the length of chain created if the specified block is placed at the
	 * supplied co-ordinates.
	 * @param x The x co-ordinate of the block.
	 * @param y The y co-ordinate of the block.
	 * @param block The block to place.
	 * @return The length of the chain created.
	 */
	s32 getPotentialChainLength(s32 x, s32 y, BlockBase* block) const;

	/**
	 * Populates the supplied Point references with the co-ordinates of the live
	 * blocks.
	 * @param liveBlock1 The first live block Point reference.
	 * @param liveBlock2 The second live block Point reference.
	 */
	void getLiveBlockPoints(Point& liveBlock1, Point& liveBlock2) const;

private:

	static const s32 BLOCK_EXPLODE_SCORE = 10;			/**< Points for exploding a block. */
	static const s32 GARBAGE_LAND_OFFSET = 5;			/**< Column pixel offset when a garbage block lands. */

	BlockBase** _data;			/**< Grid of blocks. */
	Point* _liveBlocks;			/**< Array of 2 Point objects that track the location of the live blocks. */
	bool _hasLiveBlocks;		/**< True if there are any live blocks in the grid. */
	s32* _columnOffsets;		/**< Y Offset of all columns caused by dropping garbage blocks. */
	
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
};

#endif
