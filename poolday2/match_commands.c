#include "match_commands.h"


int rx_address[]  = {52, 53, 54};
char package[10];

extern volatile bool active;

/* ISR(INT2_vect){
	 received_message = true;
 } */

void init_rf(int robot_num){
	while(!m_rf_open(CHANNEL, rx_address[robot_num], 10)){}
	sei();
}

void switch_sides(){
	if (color == RED) {
		color = BLUE;
	} else {
		color = RED;
	}
}

void read_switch(){
	// read input
	// set color
	if (check(PINB, 0)) {
		color = BLUE;
		m_red(OFF);
		m_green(ON);
	} else {
		color = RED;
		m_green(OFF);
		m_red(ON);
	}
}

void light_LED() {
	if (color == BLUE) {	
		// Testing LED
		set(PORTD, 5);
		clear(PORTD, 3);
	} else {	
		// Testing LED
		clear(PORTD, 5);
		set(PORTD, 3);
	}
 }

void flash_LED(){
	// turn on LED
	light_LED(color);
	m_wait(100);
	// turn off LED
	clear(PORTD, 5);
	clear(PORTD, 3);
	m_wait(100);
	light_LED(color);
}

void handle_message(){

	char package[10];
	 // read message into package buffer
	 if (!m_rf_read(package, 10)) {
		 // Default to pause if command not read
		 
		 package[0] = 0xA4;
		 } else {
	 }
	 
	 unsigned char first = (unsigned char)package[0];
	 switch(first){
		 case 0xA0: // Comm test
		 // flash LED
			 //read_switch();
			 break;
		 case 0xA1:	// Play command
			 // light LED
			 // set flag to play!
			 active = true;
			 //read_switch();
			 //flash_LED();
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
	 package[0] = 0xA4;
}