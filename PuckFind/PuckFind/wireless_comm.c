#include "wireless_comm.h"

#define CHANNEL 1

int rx_address[]  = {51, 52, 53};
char package[10];


void init_rf(int robot_num){
	while(!m_rf_open(1, rx_address[robot_num], 10)){
		m_red(TOGGLE);
		m_wait(100);
	}
	sei();
}

ISR(INT2_vect){
	// read message into package buffer
	m_rf_read(package, 10);
	switch(package[0]){
		case 0xA0: // Comm test
			// flash LED 
			break;
		case 0xA1:	// Play command
			// light LED
			// set flag to play!
			active = true;
			break;
		case 0xA2:
			break;
		case 0xA3:
			break;
		case 0xA4:
			break;
		case 0xA5:
			break;
		case 0xA6:
			break;
		case 0xA7:
			break;
	}
}