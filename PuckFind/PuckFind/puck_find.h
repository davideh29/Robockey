#ifndef puck_find_h
#define puck_find_h

#include <stdlib.h>
#include <string.h>
#include "m_general.h"
#include "motor_control.h"
#include "m_usb.h"

#define NUM_PTS 5

/**************************************************************************************************
DESCRIPTION: Read in ADC values from pins F0, F1, F4, F5, F6, F7, D4, D6, & D7

	Possible numbers of PTs (smaller numbers for testing):
		1: just in the front (puck holder)[0]
		3: one in puck holder [0], one on either side [1,2]
		5: one in puck holder [0], two on left [1,2], two on right [3,4]
		7: one in puck holder [0], three on left [1,2,3], three on right [4,5,6]
		9: three in puck holder [0,1,2], three on left [3,4,5], three on right [6,7,8]

INPUTS:
	pt_data[] - pointer to int array of length NUM_PTS

UPDATES:
	pt_data[] now holds ADC readings
	
OUTPUTS:
	none
*/

void read_pts(int pt_data[]);


/**************************************************************************************************
DESCRIPTION: Returns the direction to turn toward puck (-1, 0, 1)

	Possible numbers of PTs (smaller numbers for testing):
		1: just in the front (puck holder)[0]
		3: one in puck holder [0], one on either side [1,2]
		5: one in puck holder [0], two on left [1,2], two on right [3,4]
		7: one in puck holder [0], three on left [1,2,3], three on right [4,5,6]
		9: three in puck holder [0,1,2], three on left [3,4,5], three on right [6,7,8]
		
INPUTS:
	pt_data[] - Pointer to int array of length NUM_PTS of ADC readings

OUTPUTS:
	-1 means rotate negatively to face the puck, (CW)
	0 means the puck is ahead, no need to rotate,
	1 means rotate positively to face the puck (CCW)
*/

int get_turn(int pt_data[]);


/**************************************************************************************************
DESCRIPTION: Uses motor controls and get_turn to turn to face the puck

INPUTS:
	pt_data[] - pointer to int array of length NUM_PTS of ADC readings

OUTPUTS:
	none
*/

void turn_to_puck(int pt_data[]);



/**************************************************************************************************
DESCRIPTION: Determines if the robot has the puck.

INPUTS:
	pt_data[] - pointer to int array of length NUM_PTS of ADC readings

OUTPUTS:
	true if the robot is has possession of the puck
	false if the robot does not have possession of the puck
*/

bool has_puck(int pt_data[]);


/**************************************************************************************************
DESCRIPTION: Prints out ADC readings to USB

ASSUMES: USB connection has already been initialized

INPUTS:
	pt_data[] - pointer to int array of length 5

OUTPUTS:
	none
*/

void printADC(int pt_data[]);

#endif