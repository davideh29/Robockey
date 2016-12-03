#include "robockey_robot.h"

#ifndef motor_control_h
#define motor_control_h

void motor_init();

void motor_stop();

void turn_in_place(bool right, int speed);

// -50 to -1 left, 0 forward, 1 to 50 right
void turn(int direction);

bool facing_goal(Robot* robot, float opponent_x, float opponent_y);

#endif