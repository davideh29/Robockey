/*
 * LocalizationTest.c
 *
 * Created: 11/18/2016 5:17:08 PM
 * Author : PAZ0
 */ 

#include <avr/io.h>
#include "m_general.h"
#include "m_wii.h"
#include "m_usb.h"
#include "motor_control.h"
#include "robockey_robot.h"
#define PI 3.14159

int main(void) {
	// Wait to avoid covering mWii w/finger
	m_wait(1000);
	m_clockdivide(3);
	// Initialize motor
	motor_init();
	// Get mwii readings
	char wiiOpen = m_wii_open();
	if (wiiOpen) {
		m_green(ON);
	} else {
		m_red(ON);
	}
	unsigned int starArray[12];
	Robot testBot;
	m_usb_init();
	turn_in_place(true);
	bool facing = false;
	while (1) {
		// Get mWii array
		if (!m_wii_read(starArray)) m_red(ON);
		// Get robot location and orientation
		interpret(&testBot, starArray);
		// Wait
		m_wait(5000);
	}
}

