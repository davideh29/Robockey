#ifndef puck_find_h
#define puck_find_h

/**************************************************************************************************
DESCRIPTION: Read in ADC values from pins F0, F1, F4, F5 & F6

INPUTS:
	input[] - pointer to int array of length 5
			- Assumes:
			-> F0 - Front-right phototransistor
			-> F1 - Front-left phototransistor
			-> F4 - Right phototransistor
			-> F5 - Left phototransistor
			-> F6 - Back phototransistor
OUTPUTS:
	none
*/

void readPhototransistors(int input[]);


/**************************************************************************************************
DESCRIPTION: Returns the direction of the puck (-1, 0, 1)

INPUTS:
	input[] - Pointer to int array of length 5 of ADC readings
			- Assumes:
				-> input[0] - Front-right phototransistor
				-> input[1] - Front-left phototransistor
				-> input[2] - Right phototransistor
				-> input[3] - Left phototransistor
				-> input[4] - Back phototransistor

OUTPUTS:
	If in front, returns 0
	If to the right, returns 1
	If to the left, returns -1
*/

int isInFront(int input[]);


/**************************************************************************************************
DESCRIPTION: Uses motor controls and isInFront to turn to face the puck

INPUTS:
	input[] - pointer to int array of length 5 of ADC readings

OUTPUTS:
	none
*/

void turnToPuck(int input[]);


/**************************************************************************************************
DESCRIPTION: Prints out ADC readings to USB

ASSUMES: USB connection has already been initialized

INPUTS:
	input[] - pointer to int array of length 5

OUTPUTS:
	none
*/

void printADC(int input[]);

#endif