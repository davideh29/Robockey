#include "robockey_robot.h"

#ifndef motor_control_h
#define motor_control_h

void motor_init();

void motor_stop();

void turn_in_place(bool right);

// -50 to -1 left, 0 forward, 1 to 50 right
void turn(int direction);

bool turn_to_goal(Robot* robot, bool right);

#endif