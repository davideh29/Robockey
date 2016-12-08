/*
 * Robockey.c
 *
 * Team DAYS (13)
 */ 

 #include "robockey.h"

 void color_init() {
	// Set pins D2&3 as output
	set(DDRD, 2);
	set(DDRD, 3);
 }

 void color_set(bool red) {
	if (red) {
		// Testing LED
		set(PORTD, 2);
		clear(PORTD, 3);
	} else {
		// Testing LED
		clear(PORTD, 2);
		set(PORTD, 3);
	}
 }

int main(void){
	m_wait(3000);
	// System prescaler
	m_clockdivide(3);
	color_init();
	color_set(false);
	// Initialize usb
	m_usb_init();
	if (!m_usb_isconnected()) {
		//m_green(ON);
	}
	// m_green(OFF);
	// Initialize motor
	motor_init();
	//Initialize m_wii
	char wiiOpen = 0;
	while(wiiOpen == 0) {
		wiiOpen = m_wii_open();
		m_red(ON);
		//return 0;
	}
	m_red(OFF);
	
	Robot robot;	// Robot variable
	init_robot(&robot);

	///// FOR TESTING WITHOUT PLAY COMMAND //////////////////
	// init_rf(1);
	active = true;
	unsigned int star_array[12];
	/////////////////////////////////////////////////////////
	int k = 0;

	// Main loop
	while (1) {
		if (k++ >= 100) {
			k = 0;
			/*
			if (m_wii_read(star_array)) {
				m_green(TOGGLE);
			}
			m_green(TOGGLE);
			m_red(TOGGLE);
			interpret(&robot, star_array);
			*/
		}
		// Check if robot is active
		if(active){
			// TO TEST DRIVING TO GOAL
			if (step_to_goal(&robot)) {
				motor_stop();
				m_green(ON);
				while (1) {}
			}
			// move to puck 
			/*if(step_to_puck()){
				turn(0);
				// if step_to_puck returns true, it means you have the puck!
				if(step_to_goal(&robot)) {
					// if step_to_goal returns true, it means you scored!
					m_green(ON);
				}
			}*/
		}
	}
	
	return 0;
}




