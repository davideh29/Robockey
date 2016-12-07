/* Code by Andrea Frank (2016) */

/* ROBOCKEY ROBOT
Defines Robot structure and allows interpretation of measurements
into Robot position and orientation for further motion planning.
*/

#ifndef robockey_robot_h
#define robockey_robot_h

#include "robockey.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "float.h"
#include "m_usb.h"

typedef struct {
	bool has_puck;	// denotes if robot has possession of the puck
	float x;		// x position, cm
	float y;		// y position, cm
	float o;		// orientation, radians
} Robot;

volatile enum {BLUE, RED} color;	// assigned by switch
volatile bool active; // denotes if a game is active or paused

void init_robot(Robot* robot, int robot_num);

void set_color(int robot_color);

void play();

void pause();

// method for updating Robot data based on measurement
void interpret(Robot* robot, unsigned int measurement[]);
 
#endif