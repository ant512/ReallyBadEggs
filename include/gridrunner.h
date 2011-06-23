#ifndef _GRID_RUNNER_H_
#define _GRID_RUNNER_H_

#include <graphics.h>
#include <nds.h>

#include "blockserver.h"
#include "controllerbase.h"
#include "gamefont.h"
#include "grid.h"

/**
 * Controls a grid.  Maintains a state machine that tracks what should happen
 * currently and next as the game progresses.
 */
class GridRunner {
public:

	/**
	 * All types of game that can be played.
	 */
	enum GameType {
		GAME_TYPE_TWO_PLAYER = 0,						/**< Player vs AI. */
		GAME_TYPE_A = 1,								/**< Endless with advancing levels. */
		GAME_TYPE_B = 2									/**< Get 25 chains to win. */
	};

	/**
	 * Constructor.
	 * @param controller A controller object that will provide input for the
	 * movement of live blocks.
	 * @param grid Grid to run.
	 * @param blockServer The block server to use to produce next blocks for the
	 * grid.
	 * @param playerNumber The unique number of the player using this runner.
	 * @param x The x co-ordinate to render at.
	 * @param gameType The type of game to play.
	 */
	GridRunner(ControllerBase* controller,
			   Grid* grid,
			   BlockServer* blockServer,
			   s32 playerNumber,
			   s32 x,
			   GameType gameType);

	/**
	 * Destructor.
	 */
	~GridRunner();

	/**
	 * Process a single iteration of the state machine/grid logic.  This model
	 * enables other code to be run between iterations of the grid (for example,
	 * if two grids are running because we've got a two-player game).
	 */
	void iterate();

	/**
	 * Gets the number of outgoing garbage blocks (ie. blocks to be given to
	 * opponent).
	 * @return The number of outgoing garbage blocks.
	 */
	s32 getOutgoingGarbageCount() const;

	/**
	 * Gets the number of incoming garbage blocks (ie. blocks to add to the grid
	 * when the live blocks have landed).
	 * @return The number of incoming garbage blocks.
	 */
	s32 getIncomingGarbageCount() const;

	/**
	 * Increase the amount of incoming garbage blocks by the specified amount.
	 * Garbage can only be added when the grid runner is in its "live" state.
	 * @param count The number of incoming garbage blocks to increase by.
	 * @return True if the garbage was added; false if not.
	 */
	bool addIncomingGarbage(s32 count);

	/**
	 * Resets the number of outgoing garbage blocks to 0.
	 */
	void clearOutgoingGarbageCount();

	/**
	 * Gets the grid that the runner is controlling.
	 * @return The grid.
	 */
	const Grid* getGrid() const;

	/**
	 * Check if the game is over for this grid runner.
	 * @return True if the game is over.
	 */
	bool isDead() const;

private:

	/**
	 * All possible states of the state machine.
	 */
	enum GridRunnerState {
		GRID_RUNNER_STATE_DROP = 0,			/**< Blocks are dropping automatically. */
		GRID_RUNNER_STATE_LIVE = 1,			/**< Live, user-controlled blocks are in play. */
		GRID_RUNNER_STATE_LANDING = 2,		/**< Blocks are running their landing animations. */
		GRID_RUNNER_STATE_EXPLODING = 3,	/**< Blocks are running their exploding animations. */
		GRID_RUNNER_STATE_DEAD = 4,			/**< Game is over; player has lost. */
		GRID_RUNNER_STATE_WON = 5			/**< Game is over; player has won. */
	};

	static const s32 AUTO_DROP_TIME = 2;	/**< Frames between drops when blocks are automatically dropping. */
	static const s32 LIVE_DROP_TIME = 40;	/**< Frames between forced drops of live blocks. */
	static const s32 GAME_TYPE_B_START_CHAINS = 25;		/**< Number of chains to remove in game type B. */
	static const s32 CHAIN_SEQUENCE_GARBAGE_BONUS = 6;	/**< Bonus garbage blocks added for each chain sequence. */

	Grid* _grid;							/**< The grid the runner controls. */
	GridRunnerState _state;					/**< The state of the state machine. */
	s32 _timer;								/**< Frames since the last event took place. */
	ControllerBase* _controller;			/**< Controller that feeds user input to live blocks. */
	BlockServer* _blockServer;				/**< Produces next blocks for the grid. */
	BlockBase** _nextBlocks;				/**< Array of 2 blocks that will be placed next. */
	s32 _playerNumber;						/**< Unique number of the player using this runner. */
	s32 _x;									/**< The x co-ordinate to render at. */

	s32 _score;								/**< Current score. */
	s32 _level;								/**< Current level. */
	s32 _chains;							/**< Number of chains exploded. */
	s32 _scoreMultiplier;					/**< Increases when multiple chains are exploded in one move. */

	s32 _pendingGarbageCount;				/**< Number of incoming/outgoing garbage blocks.  Negative indicates
												 outgoing; positive indicates incoming. */
	s32 _outgoingGarbageCount;				/**< Outgoing garbage blocks that accumulate during chain
												 sequences.  At the end of a sequence they are moved to the
												 _pendingGarbageCount member. */

	GameType _gameType;						/**< The type of game being played. */

	GameFont _font;							/**< Font used for rendering text. */

	/**
	 * Draws the current score at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderScore(s32 x, s32 y);

	/**
	 * Draws the current level number at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderLevelNumber(s32 x, s32 y);

	/**
	 * Draws the current chain count at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderChainCount(s32 x, s32 y);

	/**
	 * Draws the two next blocks at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderNextBlocks(s32 x, s32 y) const;

	void renderOutgoingGarbage(s32 x, s32 y);
	void renderIncomingGarbage(s32 x, s32 y);

	void drop();
	void land();
	void live();

	/**
	 * Check if the grid can receive garbage.  Grid can only receive garbage
	 * whilst in the live state.  If garbage is received at other times it is
	 * possible that the player will forever be stuck watching garbage dropping
	 * down the screen.
	 * @return True if the grid can receive garbage.
	 */
	bool canReceiveGarbage() const;
};

#endif
