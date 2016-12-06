/*
 * Robockey.c
 *
 * Team DAYS (13)
 */ 

 #include "robockey.h"

int main(void){
	m_wait(3000);
	// System prescaler
	m_clockdivide(3);
	// Initialize usb
	m_usb_init();
	// Initialize motor
	motor_init();
	//Initialize m_wii
	char wiiOpen = m_wii_open();
	while(!wiiOpen) {
		m_red(ON);
		//return 0;
	}

	
	Robot robot;	// Robot variable
	init_robot(&robot, 1);

	///// FOR TESTING WITHOUT PLAY COMMAND //////////////////
	// init_rf(1);
	active = true;
	unsigned int star_array[12];
	/////////////////////////////////////////////////////////
	int k = 0;
	while (1) {
		if (k++ >= 40) {
			k = 0;
			m_wii_read(star_array);
			interpret(&robot, star_array);
		}
		// Check if robot is active
		if(active){
			// move to puck
			if(step_to_puck()){
				motor_stop();
				m_wait(2000);
				// if step_to_puck returns true, it means you have the puck!
				if(step_to_goal(&robot)) {
					// if step_to_goal returns true, it means you scored!
					m_green(ON);
				}
			}
		}
	}
	
	return 0;
}




