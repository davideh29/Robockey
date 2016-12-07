/*
 * Robockey.c
 *
 * Team DAYS (13)
 */ 

 #include "robockey.h"

 #define ROBOT_NUM 0
 
 void color_init() {
	// Set pins D2&3 as output
	set(DDRD, 2);
	set(DDRD, 3);
 }


 void robockey_init(){
	// System prescaler
	m_clockdivide(3);
	color_init();
	// Initialize usb
	m_usb_init();
	// Initialize motor
	motor_init();
	//Initialize m_wii
	char wiiOpen = 0;
	while(wiiOpen == 0) {
		wiiOpen = m_wii_open();
		m_red(ON);
	}
	m_red(OFF);
	
 }
 
int main(void){
	m_wait(3000);
	// READ COLOR FROM SWITCH
	robockey_init();
	
	Robot robot;	// Robot variable
	init_robot(&robot, ROBOT_NUM);

	///// FOR TESTING WITHOUT PLAY COMMAND //////////////////
	// init_rf(1);
	active = true;
	unsigned int star_array[12];
	/////////////////////////////////////////////////////////
	int k = 0;

	// Main loop
	while (1) {
		// print stuff
		if (k++ >= 100) {
			k = 0;
			if (m_wii_read(star_array)) {
				m_green(TOGGLE);
			}
			//m_wii_read(star_array);
			m_red(TOGGLE);
			interpret(&robot, star_array);
		}
		
		
		
		// Check if robot is active
		if(active){
			// move to puck 
			if(step_to_puck()){
				// if step_to_puck returns true, it means you have the puck!
				turn(0);
				if(step_to_goal(&robot)) {
					// if step_to_goal returns true, it means you scored!
					m_red(ON);
				}
			}
		}
		
		if(has_message){
			handle_message();
		}
	}
	
	return 0;
}




