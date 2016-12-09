#include "score.h"

bool step_to_goal(Robot* robot) {
	unsigned int star_array[12];
	// Get mWii array
	m_wii_read(star_array);
	// Get robot location and orientation
	interpret(robot, star_array);
	// Wait
	if (facing_goal(robot, OPPONENT_X, OPPONENT_Y)) {
	m_green(ON);
		turn(0); // drive forward
	} else {
		m_green(OFF);
		turn_in_place(true, 100);
	}
	/*turn(drive_to_goal(robot, OPPONENT_X, OPPONENT_Y));*/
	float dist = powf(powf(OPPONENT_X - robot->x, 2.0) + powf(OPPONENT_Y - robot->y, 2.0), 0.5);
	m_usb_tx_string("Distance: ");
	m_usb_tx_int(dist);
	m_usb_tx_char(13);
	// if ( abs(OPPONENT_Y - robot->y) > 15 || abs(OPPONENT_X - robot->x) > 30) {
	if (dist < 20) {
		// you're in the goal
		//motor_stop();
		return true;
	}
	 return false;
 }
