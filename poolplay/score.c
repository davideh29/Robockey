#include "score.h"

 bool step_to_goal(Robot* robot) {
	 unsigned int star_array[12];
	 // Get mWii array
	 m_wii_read(star_array);
	 // Get robot location and orientation
	 interpret(robot, star_array);
	 // Wait
	 if (facing_goal(robot, OPPONENT_X, OPPONENT_Y)) {
		 turn(0); // drive forward
	 } else {
		turn_in_place(true, 100);
	 }
	 if ( abs(OPPONENT_Y - robot->y) > 15 || abs(OPPONENT_X - robot->x) > 30) {
		 // you're in the goal
		 motor_stop();
		 m_red(ON);
		 return true;
	 }
	 return false;
 }
