#ifndef _BLOCK_SERVER_H_
#define _BLOCK_SERVER_H_

#include <nds.h>
#include <woopsiarray.h>

#include "blockbase.h"

/**
 * Each time a new set of blocks is added to a grid, the grid asks an instance
 * of this clss for the blocks.  All grids must share the same BlockServer
 * instance.
 *
 * This class maintains a list of block pairs and the position in the list of
 * each player.  Thus, player 1 could request a block pair.  If no blocks exist
 * in the list a pair of random blocks is added.  When player 2 requests a pair
 * he will receive the pair previously given to player 1.  If there are no more
 * players in the list then that pair is forgotten.  If there are more players,
 * the pair is retained in the list until all players have used it.  If player 1
 * requests 10 pairs whilst player 2 is working on his first pair, the 9 pairs
 * between the two players are retained until both players have used them.  This
 * ensures that all players are given the same set of blocks in the same order.
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
	 * Get the next pair of blocks for the specified player.
	 * @param player The number of the player for whom to fetch the next pair
	 * of blocks (player is 0-based).
	 * @param block1 Pointer-to-a-pointer-to-a-block.  The block pointer will be
	 * populated with a pointer to the new block.  This is the first block in
	 * the pair.
	 * @param block2 Pointer-to-a-pointer-to-a-block.  The block pointer will be
	 * populated with a pointer to the new block.  This is the second block in
	 * the pair.
	 */
	void getNextBlocks(s32 player, BlockBase** block1, BlockBase** block2);

	/**
	 * Empties the server block list.
	 */
	void clear();

private:

	/**
	 * List of all block types that the class can serve.
	 */
	enum BlockType {
		BLOCK_RED = 0,
		BLOCK_BLUE = 1,
		BLOCK_YELLOW = 2,
		BLOCK_PURPLE = 3,
		BLOCK_GREEN = 4,
		BLOCK_ORANGE = 5
	};

	/**
	 * A pair of block types.
	 */
	typedef struct {
		BlockType block1;
		BlockType block2;
	} BlockPair;

	WoopsiArray<BlockPair> _blockList;
	s32* _playerBlockListIndices;
	s32 _blockColourCount;
	s32 _playerCount;

	/**
	 * Adds a new pair of random blocks to the block list.
	 */
	void addRandomBlockPair();

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
