#ifndef _GRID_RUNNER_H_
#define _GRID_RUNNER_H_

#include <nds.h>

#include "grid.h"

class GridRunner {
public:
	GridRunner();
	~GridRunner();

	void iterate();

private:

	enum GridRunnerState {
		GRID_RUNNER_STATE_DROP = 0,
		GRID_RUNNER_STATE_LIVE = 1,
		GRID_RUNNER_STATE_LANDING = 2,
		GRID_RUNNER_STATE_EXPLODING = 3
	};

	static const s32 AUTO_DROP_TIME = 5;	/**< Frames between drops when blocks are automatically dropping. */
	static const s32 LIVE_DROP_TIME = 30;	/**< Frames between forced drops of live blocks. */

	Grid _grid;
	GridRunnerState _state;
	s32 _timer;
};

#endif
