
#include <stdlib.h>
#include "m_general.h"
#include "motor_control.h"
#include "m_usb.h"

// Defines the maximum ADC difference allowed between the front two while still being considered "forwards"
#define DIFFERENCE_LIM 50

bool jtag_enabled = true;

// Read in ADC values from pins F0, F1, F4, F5 & F6
void readPhototransistors(int input[]) {
	
	// disable JTAG if not disabled already, so we can use F4-7
	if(jtag_enabled){
		m_disableJTAG();
		jtag_enabled = false;
	}
	
	////// Analog to Digital Input Conversion //////
	// Set reference voltage to VCC
	clear(ADMUX, REFS1);
	set(ADMUX, REFS0);
	// Prescale clock
	set(ADCSRA, ADPS2);
	clear(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);
	// Disable digital input
	set(DIDR0, ADC0D); // F0
	set(DIDR0, ADC1D); // F1
	set(DIDR0, ADC4D); // F4
	set(DIDR0, ADC5D); // F5
	set(DIDR0, ADC6D); // F6

	// ADC to free-running
	set(ADCSRA, ADATE);
	// Set MUX -> input is F0
	clear(ADCSRA, ADEN);
	clear(ADCSRB, MUX5);
	clear(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	// Start conversion process
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	// Wait until value is read
	while (!check(ADCSRA, ADIF)) {}
	// Read value
	input[0] =  ADC;
	// Reset flag
	clear(ADCSRA, ADIF);
	m_wait(1);

	// Set MUX -> input is F1
	clear(ADCSRA, ADEN);
	set(ADMUX, MUX0);
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	while (!check(ADCSRA, ADIF)) {}
	input[1] =  ADC;
	clear(ADCSRA, ADIF);

	// Set MUX -> input is F4
	clear(ADCSRA, ADEN);
	set(ADMUX, MUX2);
	clear(ADMUX, MUX0);
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	while (!check(ADCSRA, ADIF)) {}
	input[2] =  ADC;
	clear(ADCSRA, ADIF);
	m_wait(1);

	// Set MUX -> input is F5
	clear(ADCSRA, ADEN);
	set(ADCSRB, MUX0);
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	while (!check(ADCSRA, ADIF)) {}
	input[3] =  ADC;
	clear(ADCSRA, ADIF);
	m_wait(1);

	// Set MUX -> input is F6
	clear(ADCSRA, ADEN);
	set(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	while (!check(ADCSRA, ADIF)) {}
	input[4] =  ADC;
	clear(ADCSRA, ADIF);
	m_wait(1);

}

// Returns the direction of the puck (-1, 0, 1)
	// -1 means rotate negatively to face the puck,
	// 0 means the puck is ahead, no need to rotate,
	// 1 means rotate positivesly to face the puck
int isInFront(int input[]) {
	// Get index of max reading
	int max_index = 0;
	int i;
	for (i = 1; i < 5; i++) {
		if (input[i] > input[max_index]) {
			max_index = i;
		}
	}
	// If one of the front phototransistors, get difference between them
	if (max_index == 0 || max_index == 1) {
		int difference = input[0] - input[1];
		// If difference is within difference limit, return 0
		if (abs(difference) < DIFFERENCE_LIM) {
			return 0;
		// Otherwise return direction of puck
		} else if (difference > 0) {
			return 1;
		} else {
			return -1;
		}
	}
	// If maximum reading is from right or left, return direction
	if (max_index == 2) return 1;
	if (max_index == 3) return -1;
	// If the back phototransistor find the higher value of the side readings
	if (max_index == 4) {
		if (input[2] > input[3]) {
			return 1;
		} else {
			return -1;
		}
	}
	
	return 0;
}

// Uses motor controls and isInFront to turn to face the puck
void turnToPuck(int input[]) {
	if (isInFront(input) < 0) {
		turn_in_place(false);
	} else if (isInFront(input) > 0) {
		turn_in_place(true);
	}
}

// Prints out ADC readings to usb
void printADC(int input[]) {
	m_usb_tx_string("   ---   ");
	m_usb_tx_string("F0: ");
	m_usb_tx_int(input[0]);
	m_usb_tx_string("   ---   ");
	m_usb_tx_string("F1: ");
	m_usb_tx_int(input[1]);
	m_usb_tx_string("   ---   ");
	m_usb_tx_string("F4: ");
	m_usb_tx_int(input[2]);
	m_usb_tx_string("   ---   ");
	m_usb_tx_string("F5: ");
	m_usb_tx_int(input[3]);
	m_usb_tx_string("   ---   ");
	m_usb_tx_string("F6: ");
	m_usb_tx_int(input[4]);
	m_usb_tx_string("   ---   ");
	m_usb_tx_char(13);
}
