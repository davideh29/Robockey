/*
 * Robockey.c
 *
 * Created: 11/12/2016 5:56:10 PM
 * Author : PAZ0
 */ 

#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "puck_find.h"

int main(void)
{
	// System prescaler
	m_clockdivide(3);
	// Initialize usb
	m_usb_init();
	// Initialize motor
	motor_init();
	// Array for phototransistor readings
	int input[5];
	// Find puck direction and turn to face it
	while (1) {
		readPhototransistors(input); // Get ADC phototransistor inputs
		printADC(input); // Print ADC values
		// If facing puck, turn on green light
		if (isInFront(input) == 0) {
			motor_stop();
			m_green(ON);
		} else {
			turnToPuck(input); // Turn to face puck
		}
		m_wait(500);
	}
}


