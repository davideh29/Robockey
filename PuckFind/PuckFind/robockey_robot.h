/* Code by Andrea Frank (2016) */

/* ROBOCKEY ROBOT
Defines Robot structure and allows interpretation of measurements
into Robot position and orientation for further motion planning.
*/

#ifndef robockey_robot_h
#define robockey_robot_h

#include <stdlib.h>
#include <math.h>

typedef struct {
	enum {ATTACKER, GOALIE} type;	
	bool has_puck;	// denotes if robot has possession of the puck
	float x;		// x position, cm
	float y;		// y position, cm
	float o;		// orientation, radians
} Robot;

// method for updating Robot data based on measurement
void interpret(Robot* robot, unsigned int measurement[]);
 
#endif