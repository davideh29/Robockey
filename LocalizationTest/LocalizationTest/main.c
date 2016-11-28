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
	m_clockdivide(3);
	// Initialize motor
	motor_init();
	turn(50);
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
	while (!m_usb_isconnected()){
	}
	while (1) {
		// Get mWii array
		if (!m_wii_read(starArray)) m_red(ON);
		// Get robot location and orientation
		interpret(&testBot, starArray);
		// Print results through usb
		for (int i = 0; i < 4; i++) {
			m_usb_tx_int(starArray[3*i]);
			m_usb_tx_char(95);
			m_usb_tx_int(starArray[3*i + 1]);
			m_usb_tx_char(95);
		}
		m_usb_tx_char(13);
		m_usb_tx_int((int) testBot.x);	
		m_usb_tx_char(95);
		m_usb_tx_int((int) testBot.y);	
		m_usb_tx_char(95);
		m_usb_tx_int((int) (testBot.o * 180.0 / PI));	// print angle in degrees
		m_usb_tx_char(13);
		m_usb_tx_char(13);
		m_wait(5000);
	}
}

