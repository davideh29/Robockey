#ifndef robockey_score_h
#define robockey_score_h

#include "robockey.h"
#include <stdbool.h>

#define OPPONENT_X 35
#define OPPONENT_Y 315


/**************************************************************************************************
DESCRIPTION: Calculates how to get to score (either turn in place or drive straight) and takes one
				step in that task. Also determines if you made it.

INPUTS:
	none

OUTPUTS:
	0 if not at the goal yet
	1 if at the goal
*/
bool step_to_goal(Robot* robot);



#endif