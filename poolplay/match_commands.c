#include "match_commands.h"

#define CHANNEL 1

int rx_address[]  = {51, 52, 53};
char package[10];

extern volatile bool active;

void init_rf(int robot_num){
	sei();
	while(!m_rf_open(1, rx_address[robot_num], 10)){
		m_red(TOGGLE);
		m_wait(100);
	}
}

ISR(INT2_vect){
	has_message = true;
}

void read_switch(){
	// read input
	// set color
}

void light_LED() {
	if (color) {	// red (color = 1)
		// Testing LED
		set(PORTD, 2);
		clear(PORTD, 3);
	} else {	// blue (color = 0)
		// Testing LED
		clear(PORTD, 2);
		set(PORTD, 3);
	}
 }

void flash_LED(){
	// turn off LED
	clear(PORTD, 2);
	clear(PORTD, 3);
	m_wait(100);
	// turn on LED
	light_LED(color);
}

void handle_message(){
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
			play();
			read_switch();
			flash_LED();
			break;
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA6:
		case 0xA7:
		default:
			pause();
	}
	
}