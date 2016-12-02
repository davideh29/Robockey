/*
 * Robockey.c
 *
 * Team DAYS (13)
 */ 

 #include "robockey.h"

int main(void){
	// System prescaler
	m_clockdivide(3);
	// Initialize usb
	m_usb_init();
	// Initialize motor
	motor_init();
	// Array for phototransistor readings
	int pt_data[NUM_PTS];
	
	
	// Find puck direction and turn to face it
	while (1) {
		read_pts(pt_data); // Get ADC phototransistor pt_datas
		printADC(pt_data); // Print ADC values to usb
		// If facing puck, turn on green light
		if (get_turn(pt_data) == 0) {
			motor_stop();
			m_green(ON);
		} else {
			turn_to_puck(pt_data); // Turn to face puck
		}
		m_wait(500);
	}
	
	return 0;
}


