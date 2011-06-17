Scoring
-------

 - Each exploded block in a chain is worth 10 points.
 - The score for the number of blocks exploded in a single iteration is
   multiplied by the number of chains exploded during that iteration.
 - That score is multiplied by the number of sequential chain explosions made.

The formula is:

  10 * blocks * chains * sequence

If a player explodes a chain of 4 red blocks and a chain of 5 green blocks
simultaneously, he gets a score of:

  10 * (4 + 5) * 2 * 1 = 180

He gets 10 points for each block ((4 + 5) * 10 = 90).  He made two chains
(90 * 2 = 180).  This is the first set of chains exploded (180 * 1 = 180).

The grid drops down to fill in any gaps, and in doing so creates another chain
of 4 blue blocks.  These explode giving the following score:

  10 * 4 * 1 * 2 = 80

He gets 10 points for each block (10 * 4 = 40).  He only made one chain
(40 * 1 = 40).  However, this is the second set of chains exploded, so the final
score is doubled (40 * 2 = 80).

The grid drops to fill in any gaps again, and two more chains are created: 6
yellow blocks and 5 orange blocks.  The score this time is:

  10 * (6 + 5) * 2 * 3 = 660

He gets 10 points for each block ((6 + 5) * 10 = 110).  He made two chains
(110 * 2 = 220).  This is the third set of chains created, so the score is
tripled (110 * 3 = 660)

Play then resumes, and the chain set multiplier resets.


Levels
------

Play advances through one level every time 10 chains are created.  The current
level can therefore be said to be equal to the integer result of
(total chains / 10).  Each increase in level increases the speed at which blocks
automatically drop down the screen.