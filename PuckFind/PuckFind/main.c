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
	// Array for phototransistor readings
	int pt_data[NUM_PTS];
	
	// Find puck direction and turn to face it
	int i = 0;
	while (1) {
		read_pts(pt_data); // Get ADC phototransistor pt_datas
		// If facing puck, turn on green lights
		int direction = get_turn(pt_data);
		if (++i % 40 == 0) {
			i = 0;
			printADC(pt_data); // Print ADC values to usb
			if (direction < 0) {
				m_usb_tx_string("Right");
			} else if (direction > 0) {
				m_usb_tx_string("Left");
			} else {
				m_usb_tx_string("Front");
			}
			m_usb_tx_char(13);
		}
		int med_direction = median_filter_directions(direction);
		if (med_direction == 0) {
			motor_stop();
		} else {
			turn_to_puck(med_direction); // Turn to face puck
		}
	}
	
	return 0;
}


