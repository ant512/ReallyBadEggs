#ifndef _GRID_H_
#define _GRID_H_

#include <graphics.h>
#include <nds.h>

class BlockBase;

class Grid {
public:

	static const s32 GRID_WIDTH = 6;
	static const s32 GRID_HEIGHT = 12;
	static const s32 MAX_CONNECTIONS = 4;

	Grid();
	~Grid();

	void render(WoopsiGfx::Graphics* gfx);
	void iterate();

	BlockBase* getBlockAt(s32 x, s32 y) const;
	void setBlockAt(s32 x, s32 y, u8 block);

	void clear();

private:

	BlockBase** _data;

	/**
	 * Removes all blocks that are currently connected to at least 3 others.
	 * This should be called repeatedly after blocks fall to their new positions
	 * until it returns false.  This ensures that all connected blocks are
	 * removed correctly.
	 * @return True if any blocks were removed.
	 */
	bool removeConnectedBlocks();

	s32 identifyConnections(s32 x, s32 y, s32 connectionCount, u8* connectionData);
};

#endif
