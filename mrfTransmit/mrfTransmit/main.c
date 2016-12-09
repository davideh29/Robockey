/*
 * mrfTransmit.c
 *
 * Created: 12/7/2016 8:25:59 PM
 * Author : PAZ0
 */ 

#include <avr/io.h>
#include "m_general.h"
#include "m_rf.h"
#include "m_bus.h"


int main(void)
{
	m_clockdivide(3);
	m_bus_init();
	if (!m_rf_open(25, 52, 10)) m_red(ON);
	char buffer[10] = {0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0};
	// char buffer[10] = {0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1};
    /* Replace with your application code */
    while (1) 
    {
		if (m_rf_send(52, buffer, 10)) {
			m_green(ON);
		}
		m_wait(200);
		m_green(OFF);
		m_wait(200);
	}
}

