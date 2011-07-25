#ifndef _BLOCK_SERVER_H_
#define _BLOCK_SERVER_H_

#include <nds.h>
#include <woopsiarray.h>

#include "blockbase.h"

/**
 * Each time a new set of blocks is added to a grid, the grid asks an instance
 * of this class for the blocks.  All grids must share the same BlockServer
 * instance.
 *
 * This class maintains a list of blocks and the position in the list of
 * each player.  Thus, player 1 could request a block.  If no blocks exist in
 * the list a random block is added.  When player 2 requests a block he will
 * receive the block previously given to player 1.  If there are no more
 * players in the list then that block is forgotten.  If there are more players,
 * the block is retained in the list until all players have used it.  If player
 * 1 requests 10 blocks whilst player 2 is working on his first set of blocks,
 * the 9 blocks between the two players are retained until both players have
 * used them.  This ensures that all players are given the same set of blocks in
 * the same order.
 */
class BlockServer {
public:

	/**
	 * Constructor.
	 * @param playerCount The number of players in the game.
	 * @param blockColourCount The number of block colours available.
	 */
	BlockServer(s32 playerCount, s32 blockColourCount);

	/**
	 * Destructor.
	 */
	~BlockServer();

	/**
	 * Get a new block for the specified player.
	 * @param player The number of the player for whom to fetch the next pair
	 * of blocks (number is 0-based).
	 */
	BlockBase* newBlock(s32 player);

	/**
	 * Empties the server block list.
	 */
	void clear();

private:

	/**
	 * List of all block types that the class can serve.
	 */
	enum BlockType {
		BLOCK_RED = 0,			/**< Red block. */
		BLOCK_BLUE = 1,			/**< Blue block. */
		BLOCK_YELLOW = 2,		/**< Yellow block. */
		BLOCK_PURPLE = 3,		/**< Purple block. */
		BLOCK_GREEN = 4,		/**< Green block. */
		BLOCK_ORANGE = 5		/**< Orange block. */
	};

	WoopsiArray<BlockType> _blockList;	/**< List of blocks that get served to players. */
	s32* _playerBlockListIndices;		/**< Each item in the array represents the index within
											 _blockList that each player is currently using. */
	s32 _blockColourCount;				/**< Number of colours that the server can produce. */
	s32 _playerCount;					/**< Number of players in the game. */

	/**
	 * Adds a new random block to the block list.
	 */
	void addRandomBlock();

	/**
	 * Creates a BlockBase object from a BlockType value.
	 * @param type Type of block to create.
	 * @return Pointer to the new block.
	 */
	BlockBase* newBlockFromType(BlockType type) const;

	/**
	 * Removes any blocks that have been used by all players from the list.
	 */
	void expireUsedBlocks();

	/**
	 * Gets a random block type.
	 * @return A random block type.
	 */
	BlockType getRandomBlockType() const;
};

#endif
