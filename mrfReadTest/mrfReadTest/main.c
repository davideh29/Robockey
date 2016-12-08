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

volatile bool active = false;
char buffer[10] = {0,0,0,0,0,0,0,0,0,0};


ISR (INT2_vect) {
	m_rf_read(buffer, 10);
	m_green(ON);
	m_wait(200);
}

int main(void)
{
	m_clockdivide(3);
	m_bus_init();
	m_red(ON);
	m_wait(2000);
	while (!m_rf_open(1, 65, 10)) { }
	sei();
	m_red(OFF);
    /* Replace with your application code */
    while (1) 
    {
		m_green(OFF);
		if (buffer[0] == 0xA0) {
			m_red(ON);
		}
		m_wait(200);
	}
}

