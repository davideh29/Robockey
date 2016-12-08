/* 
Robockey Team 13 - DAYS
	Andrea Frank, Sam Gaardsmoe, David Hernandez, Young Shin

Header file for that has all necessary includes
	for main method.
*/

#ifndef robockey_h
#define robockey_h

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include "robockey_robot.h"
#include "m_general.h"
#include "m_usb.h"
#include "m_rf.h"
#include "m_wii.h"
#include "match_commands.h"
#include "puck_find.h"
#include "motor_control.h"
#include "robockey_median_filter.h"
#include "score.h"

// speed factor
#define TURN_SPEED_FACTOR 150
// returns sign of numbers
#define SIGN_OF(x) (1-(x<0)*2 - (x==0))


#endif