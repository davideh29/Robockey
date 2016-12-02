#include "m_general.h"
#include "robockey_robot.h"
#define PI 3.14159

void motor_init() {
	// Digital output pins
	set(DDRB, 1); // A-IN 1
	set(DDRB, 2); // A-IN 2
	set(DDRB, 3); // B-IN 1
	set(DDRB, 7); // B-IN 2

	set(PORTB, 1);
	clear(PORTB, 2);
	set(PORTB, 3);
	clear(PORTB, 7);

	// Timer 1
	// Prescale timer clock
	clear(TCCR1B, CS12);
	set(TCCR1B, CS11);
	clear(TCCR1B, CS10);

	// Set timer mode (UP to 0x00FF, PWM mode)
	clear(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	clear(TCCR1A, WGM11);
	set(TCCR1A, WGM10);

	// Set Channel A compare output (Set at OCR1A, clear at rollover)
	set(TCCR1A, COM1A1);
	set(TCCR1A, COM1A0);

	// Set Channel B compare output (Set at OCR1B, clear at rollover)
	set(TCCR1A, COM1B1);
	set(TCCR1A, COM1B0);

	// Initialize OCR1A/B
	OCR1A = 0xBF;
	OCR1B = 0xBF;

	// Enable B5/6 output
	set(DDRB, 6);
	set(DDRB, 5);
}

void motor_stop() {
	clear(PORTB, 1);
	clear(PORTB, 2);
	clear(PORTB, 3);
	clear(PORTB, 7);
}

void turn_in_place(bool right) {
	if (right) {
		OCR1A = 0xBF;
		OCR1B = 0xBF;
		set(PORTB, 1);
		clear(PORTB, 2);
		clear(PORTB, 3);
		set(PORTB, 7);
	} else {
		OCR1A = 0xBF;
		OCR1B = 0xBF;
		clear(PORTB, 1);
		set(PORTB, 2);
		set(PORTB, 3);
		clear(PORTB, 7);
	}
}

// -50 to -1 left, 0 forward, 1 to 50 right
void turn(int direction) {
	clear(PORTB, 1);
	set(PORTB, 2);
	clear(PORTB, 3);
	set(PORTB, 7);
	if (direction == 0) {
		OCR1A = 0x37;
		OCR1B = 0x3F;
	} else if (direction > 0) {
		OCR1A = 0x00;
		OCR1B = (int) (255.0 * (float) direction / 50.0);
	} else {
		OCR1A = (int) (255.0 * (float) -direction / 50.0);
		OCR1B = 0x00;
	}
}

// Turn to face opponent's goal
bool facing_goal(Robot* robot, float opponent_x, float opponent_y) {
	float opponent_angle = (PI - atan2f(-(robot->y - opponent_y), robot->x - opponent_x));
	float error = PI / 45.0;
	float low_bound = opponent_angle - error;
	float high_bound = opponent_angle + error;
	/*
	if (low_bound < -PI) low_bound += 2.0 * PI;
	if (high_bound > PI) high_bound -= 2.0 * PI;
	// Near PI
	if (opponent_angle > low_bound && opponent_angle < PI && high_bound <= -PI + error) {
		return true;
	}
	// Near -PI
	if (opponent_angle < high_bound && opponent_angle < PI && low_bound >= PI - error) {
		return true;
	}*/
	// Normal case
	return (robot->o > low_bound && robot->o < high_bound);
}

void drive_to_goal();