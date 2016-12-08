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


 void robockey_init() {
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
	// enable B0 for digital input from switch
	clear(DDRB, 0);
	init_rf(ROBOT_NUM);
 }

 ISR(INT2_vect){
	 char package[10];
	 // read message into package buffer
	 if (!m_rf_read(package, 10)) {
		 // Default to pause if command not read
		 package[0] = 0xA4;
		 } else {
		 m_green(ON);
	 }
	 
	 switch(package[0]){
		 case 0xA0: // Comm test
		 // flash LED
		 read_switch();
		 flash_LED();
		 break;
		 case 0xA1:	// Play command
		 // light LED
		 // set flag to play!
		 active = true;
		 read_switch();
		 flash_LED();
		 turn(0);
		 break;
		 case 0xA2:
		 case 0xA3:
		 case 0xA4:
		 case 0xA6:
		 case 0xA7:
		 default:
		 active = false;
	 }
	 received_message = false;
	 package[0] = 0;
 }

int main(void){
	m_wait(3000);
	robockey_init();
	
	Robot robot;	// Robot variable
	init_robot(&robot);

	///// FOR TESTING WITHOUT PLAY COMMAND //////////////////
	active = false;
	// init_rf(1);
	//unsigned int star_array[12];
	/////////////////////////////////////////////////////////
	int k = 0;
	sei();
	while(!m_rf_open(1, 52, 10)){
		m_red(TOGGLE);
		m_wait(100);
	}

	// Main loop
	while (1) {
		// print stuff
		/*if (k++ >= 100) {
			k = 0;
			if (m_wii_read(star_array)) {
				m_green(TOGGLE);
			}
			//m_wii_read(star_array);
			m_red(TOGGLE);
			interpret(&robot, star_array);
		}*/
		
		// Check if robot is active
		if(active){
			// move to puck 
			if(step_to_puck()){
				// if step_to_puck returns true, it means you have the puck!
				turn(0);
				if(step_to_goal(&robot)) {
					// if step_to_goal returns true, it means you scored!
					m_red(ON);	// roxanne, you have to turn on the red light
				}
			}
		}
		
		
	}
	
	return 0;
}




