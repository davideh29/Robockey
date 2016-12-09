/*
 * Robockey.c
 *
 * Team DAYS (13)
 */ 

 #include "robockey.h"

 #define ROBOT_NUM 0
 
 void color_init() {
	// Set pins D5&3&3 as output
	set(DDRD, 5);
	set(DDRD, 3);
	// enable B0 for digital input from switch
	clear(DDRB,0);
	clear(PORTB,0);

 }


 void robockey_init() {
	// System prescaler
	m_clockdivide(3);
	//motor_init();
	//color_init();
	motor_init();
	// Initialize usb
	//m_usb_init();
	// Initialize motor
	
	//Initialize m_wii
	char wiiOpen = 0;
	while(wiiOpen == 0) {
		wiiOpen = m_wii_open();
	} 
	
	init_rf(ROBOT_NUM);
 }

 volatile bool received_message = false;

 
 ISR(INT2_vect){
	received_message = true;
 }
 

int main(void){

	robockey_init();
	
	Robot robot;	// Robot variable
	init_robot(&robot);
	active = false;
	

	
	// Main loop
	while (1) {
		
		m_wait(100);
		// print stuff
		/*if (k++ >= 100) {
			k = 0;
			if (m_wii_read(star_array)) {
				m_green(TOGGLE);
			}
			//m_wii_read(star_array);
			m_red(TOGGLE);
			interpret(&robot, star_array);
		} */
		
		// Check if robot is active
	if(active){
			
			// move to puck 
			if(step_to_puck()){
				// if step_to_puck returns true, it means you have the puck!
				m_green(ON);
				turn(0);
				if(step_to_goal(&robot)) {
					// if step_to_goal returns true, it means you scored!
					m_red(ON);	// roxanne, you have to turn on the red light
				}
			}
		}	
		

		if(received_message){
			handle_message();
			received_message = false;
		}
		
	}
	
	return 0;
}




