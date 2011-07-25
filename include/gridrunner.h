#ifndef _GRID_RUNNER_H_
#define _GRID_RUNNER_H_

#include <graphics.h>
#include <nds.h>

#include "blockfactory.h"
#include "controllerbase.h"
#include "gamefont.h"
#include "grid.h"

#include "smallboulderbmp.h"
#include "largeboulderbmp.h"
#include "faceboulderbmp.h"

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
		GAME_TYPE_SINGLE_PLAYER = 1,					/**< Single player. */
	};

	/**
	 * Constructor.
	 * @param controller A controller object that will provide input for the
	 * movement of live blocks.
	 * @param grid Grid to run.
	 * @param blockFactory The block factory to use to produce next blocks for
	 * the grid.
	 * @param playerNumber The unique number of the player using this runner.
	 * @param x The x co-ordinate to render at.
	 * @param gameType The type of game to play.
	 * @param speed The auto drop speed.
	 */
	GridRunner(ControllerBase* controller,
			   Grid* grid,
			   BlockFactory* blockFactory,
			   s32 playerNumber,
			   s32 x,
			   GameType gameType,
			   s32 speed);

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

	/**
	 * Get the current score.
	 * @return The current score.
	 */
	s32 getScore() const;

	/**
	 * Get the number of chains created.
	 * @return The number of chains created.
	 */
	s32 getChains() const;

	/**
	 * Get the current speed.
	 * @return The current speed.
	 */
	s32 getSpeed() const;

	/**
	 * Draws the stats (score, chains, speed, etc) to the bottom screen.
	 */
	void renderHUD();

private:

	/**
	 * All possible states of the state machine.
	 */
	enum GridRunnerState {
		GRID_RUNNER_STATE_DROP = 0,			/**< Blocks are dropping automatically. */
		GRID_RUNNER_STATE_LIVE = 1,			/**< Live, user-controlled blocks are in play. */
		GRID_RUNNER_STATE_LANDING = 2,		/**< Blocks are running their landing animations. */
		GRID_RUNNER_STATE_EXPLODING = 3,	/**< Blocks are running their exploding animations. */
		GRID_RUNNER_STATE_DEAD = 4,			/**< Game is over. */
	};

	static const s32 AUTO_DROP_TIME = 2;	/**< Frames between drops when blocks are automatically dropping. */
	static const s32 CHAIN_SEQUENCE_GARBAGE_BONUS = 6;	/**< Bonus garbage blocks added for each chain sequence. */
	static const s32 GARBAGE_FACE_BOULDER_VALUE = 24;	/**< Number of garbage blocks represented by a face boulder. */
	static const s32 GARBAGE_LARGE_BOULDER_VALUE = 6;	/**< Number of garbage blocks represented by a large boulder. */

	static const s32 SPEED_COUNT = 10;		/**< Number of items in the drop speed array. */
	static const s32 SPEEDS[SPEED_COUNT];	/**< Array of auto drop speeds. */

	Grid* _grid;							/**< The grid the runner controls. */
	GridRunnerState _state;					/**< The state of the state machine. */
	s32 _timer;								/**< Frames since the last event took place. */
	ControllerBase* _controller;			/**< Controller that feeds user input to live blocks. */
	BlockFactory* _blockFactory;			/**< Produces next blocks for the grid. */
	BlockBase** _nextBlocks;				/**< Array of 2 blocks that will be placed next. */
	s32 _playerNumber;						/**< Unique number of the player using this runner. */
	s32 _x;									/**< The x co-ordinate to render at. */

	s32 _score;								/**< Current score. */
	s32 _speed;								/**< Current speed. */
	s32 _chains;							/**< Number of chains exploded. */
	s32 _scoreMultiplier;					/**< Increases when multiple chains are exploded in one move. */

	s32 _accumulatingGarbageCount;			/**< Outgoing garbage blocks that accumulate during chain
												 sequences.  At the end of a sequence they are moved to the
												 _outgoinggGarbageCount member. */
	s32 _outgoingGarbageCount;				/**< Number of garbage blocks to send to the other player. */
	s32 _incomingGarbageCount;				/**< Number of garbage blocks sent from the other player. */

	GameType _gameType;						/**< The type of game being played. */

	GameFont _font;							/**< Font used for rendering text. */

	FaceBoulderBmp _faceBoulderBmp;			/**< Bitmap representing 24 incoming garbage blocks. */
	SmallBoulderBmp _smallBoulderBmp;		/**< Bitmap representing 1 incoming garbage block. */
	LargeBoulderBmp _largeBoulderBmp;		/**< Bitmap representing 6 incoming garbage blocks. */

	bool _droppingLiveBlocks;				/**< True if live blocks are dropping automatically. */

	/**
	 * Draws the current score at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderScore(s32 x, s32 y);

	/**
	 * Draws the current speed at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderSpeed(s32 x, s32 y);

	/**
	 * Draws the current chain count at the specified co-ordinates to the lower
	 * screen.
	 * @param x The x co-ordinate to draw at.
	 * @param y The y co-ordinate to draw at.
	 */
	void renderChainCount(s32 x, s32 y);

	/**
	 * Draws the two next blocks to the lower screen.
	 */
	void renderNextBlocks() const;

	/**
	 * Renders the incoming garbage count to the bottom screen.
	 */
	void renderIncomingGarbage();

	/**
	 * Drops blocks in the grid.  Called when the grid is in drop mode.
	 */
	void drop();

	/**
	 * Lands blocks in the grid.  Called when the grid is in land mode.
	 */
	void land();

	/**
	 * Process live blocks in the grid.  Called when the grid is in live mode.
	 */
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
