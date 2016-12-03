

#include "puck_find.h"

// Defines the minimum single PT reading to be considered "active"
#define PT_ACTIVE_THRESHOLD 30
// Minimum difference between PTs on either side to be considered not straight ahead
#define PT_DIFFERENCE_THRESHOLD 30
// Minimum reading of a single front PT when puck is in possession
#define PUCK_POSSESSION_THRESHOLD 950
// Minimum difference reading of the angled front PT's to not be considered straight ahead
#define PT_FRONT_DIFFERENCE_THRESHOLD 50

// returns sign of numbers
#define SIGN_OF(x) (1-(x<0)*2)

// returns the designated bit of a number
#define BIT0(integer) (integer & 0x01)
#define BIT1(integer) ((integer & 0x02)>>1)
#define BIT2(integer) ((integer & 0x04)>>2)
#define BIT3(integer) ((integer & 0x08)>>3)

bool jtag_enabled = true;

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

void ADC_init(){
	// Set reference voltage to VCC
	clear(ADMUX, REFS1);
	set(ADMUX, REFS0);
	// Prescale clock
	set(ADCSRA, ADPS2);
	clear(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);
	
	// Disable digital pt_datas
	set(DIDR0, ADC0D); // F0
	if(NUM_PTS > 1) {set(DIDR0, ADC1D);} // F1
	if(NUM_PTS > 2) {set(DIDR0, ADC4D);} // F4
	if(NUM_PTS > 3) {set(DIDR0, ADC5D);} // F5
	if(NUM_PTS > 4) {set(DIDR0, ADC6D);} // F6
	if(NUM_PTS > 5) {set(DIDR0, ADC7D);} // F7
	if(NUM_PTS > 6) {set(DIDR2, ADC8D);} // D4
	if(NUM_PTS > 7) {set(DIDR2, ADC9D);} // D6
	if(NUM_PTS > 8) {set(DIDR2, ADC10D);} // D7
	
	// ADC to free-running
	set(ADCSRA, ADATE);
}

/**********************************************************/
// sets MUX for ADC 
void setMUX(int ADC_num){
	if(ADC_num == 0 ||	// make sure its a valid MUX number
	 ADC_num == 1 || ADC_num == 4 ||
	 ADC_num == 5 || ADC_num == 6 ||
	 ADC_num == 7 || ADC_num == 8 ||
	 ADC_num == 9 || ADC_num == 10 ){
		if(BIT0(ADC_num)==1){
			set(ADMUX,MUX0);
		} else{
			clear(ADMUX, MUX0);
		}
		if(BIT1(ADC_num)==1){
			set(ADMUX,MUX1);
		} else{
			clear(ADMUX, MUX1);
		}
		if(BIT2(ADC_num)==1){
			set(ADMUX,MUX2);
		} else{
			clear(ADMUX, MUX2);
		}
		if(BIT3(ADC_num)==1){
			set(ADCSRB, MUX5);
		} else{
			clear(ADCSRB, MUX5);
		}
	} else {
		m_green(ON);
		// ya fucked up
	}
}

/**********************************************************/
// starts conversion using MUX set in setMUX() function,
// returns ADC value, stops conversion
int readADC(){
	// start conversion
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	// wait till value is read
	while (!check(ADCSRA, ADIF)) {}
	// store value
	int ans = ADC;
	// reset flag
	clear(ADCSRA, ADIF);
	// disable conversion
	clear(ADCSRA, ADEN);
	
	return ans;
}

/**********************************************************/
// Read in ADC values from ADC pins
	// order = D4, D6, D7, F0, F1, F4, F5, F6, F7
void read_pts(int pt_data[]) {
	
	// disable JTAG if not disabled already, so we can use F4-7
	if(jtag_enabled){
		m_disableJTAG();
		jtag_enabled = false;
	}
	
	////// Analog to Digital pt_data Conversion //////
	
	// initialize
	ADC_init(NUM_PTS);

	
	int adc_nums[] = {8, 9, 10, 0, 1, 4, 5, 6, 7};	// D4, D6, D7, F0, F1, F4, F5, F6, F7
	// read
	int i;
	for(i=0; i<NUM_PTS; i++){
		setMUX(adc_nums[i]);
		pt_data[i] = readADC(); 
		m_wait(2);
	}
	
}



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

int get_turn(int pt_data[]) {
	int direction = 0;
	int front;
	int i;
	int num_front_pts;
	switch(NUM_PTS) {
		case 1 :
		// if there's only one PT, you just want to know if you sense it ahead
			if(pt_data[0] > PT_ACTIVE_THRESHOLD){
				// puck is ahead
				return 0;
			}
			return 1;	// if not ahead, keep turning till you find it
			
		// otherwise, you're gonna want to sum the front ones to get front intensity,
		// 	and add left and subtract right to get direction to rotate
		case 9 : 
			front = pt_data[0] + pt_data[1] + pt_data[2];
			num_front_pts = 3;
			direction += pt_data[3];		// left front
			direction += 2 * pt_data[4];	// left middle
			direction += 3 * pt_data[5];	// left back
			direction -= 3 * pt_data[6];	// right back
			direction -= 2 * pt_data[7];	// right middle
			direction -= 1 * pt_data[8];	// right front
			break;
		default :
			front = pt_data[0];
			num_front_pts = 1;
			for(i=1;i<NUM_PTS/2+1;i++) {	// integer divide on purpose
				direction += pt_data[i];	// add left values
			}
			for(i=NUM_PTS/2+1; i<NUM_PTS; i++) {
				direction -= pt_data[i];	// subtract right values
			}	
	}
	
	if(abs(direction) > PT_DIFFERENCE_THRESHOLD) {	// if one side is significantly brighter than the other 
		if (direction < 0) {
			m_usb_tx_string("Right");
		} else {
			m_usb_tx_string("Left");
		}
		m_usb_tx_char(13);
		return 150 * SIGN_OF(direction);
	}
	else if( front > PT_ACTIVE_THRESHOLD*num_front_pts) {
		// subtracts 0 & 2 and see direction
		int front_direction = pt_data[0] - pt_data[2];
		if (abs(front_direction) > PT_FRONT_DIFFERENCE_THRESHOLD) {
			if (front_direction < 0) {
				m_usb_tx_string("Right Front");
			} else {
				m_usb_tx_string("Left Front");
			}
			m_usb_tx_char(13);
			return 40*SIGN_OF(front_direction);
		}
		m_usb_tx_string("Front");
		m_usb_tx_char(13);
		return 0;
	}
	
	// if not on either side and front doesn't detect it, just spin until you find it
	m_usb_tx_string("Back/Too Low");
	m_usb_tx_char(13);
	return 150;
}

/**************************************************************************************************
DESCRIPTION: Determines if the robot has the puck.

INPUTS:
	pt_data[] - pointer to int array of length NUM_PTS of ADC readings

OUTPUTS:
	true if the robot is has possession of the puck
	false if the robot does not have possession of the puck
*/

bool has_puck(int pt_data[]){
	int front, num_front_pts;
	
	// get intensity of front PT(s)
	switch(NUM_PTS) {
		case 9 : 
			front = pt_data[0] + pt_data[1] + pt_data[2];
			num_front_pts = 3;
			break;
		default :
			front = pt_data[0];
			num_front_pts = 1;
	}
	
	// if the front intensity is great enough, puck is in possession
	if( front > PUCK_POSSESSION_THRESHOLD*num_front_pts ){
		return true;
	}
	
	// otherwise, it is not
	return false;
}


/**************************************************************************************************
DESCRIPTION: Uses motor controls and get_turn to turn to face the puck

INPUTS:
	pt_data[] - pointer to int array of length NUM_PTS of ADC readings

OUTPUTS:
	none
*/


void turn_to_puck(int direction) {
	if (direction < 0) {
		turn_in_place(false, direction);
	} else if (direction > 0) {
		turn_in_place(true, direction);
	}
}


/**************************************************************************************************
DESCRIPTION: Prints out ADC readings to USB

ASSUMES: USB connection has already been initialized

INPUTS:
	pt_data[] - pointer to int array of length 5

OUTPUTS:
	none
*/

void printADC(int pt_data[]) {

	m_usb_tx_string("   ---   ");
	m_usb_tx_string("D4: ");
	m_usb_tx_int(pt_data[0]);
	if(NUM_PTS > 1){
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("D6: ");
		m_usb_tx_int(pt_data[1]);
	} if(NUM_PTS > 2) {
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("D7: ");
		m_usb_tx_int(pt_data[2]);
	} if(NUM_PTS > 3){
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("F0: ");
		m_usb_tx_int(pt_data[3]);
	} if(NUM_PTS > 4) {
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("F1: ");
		m_usb_tx_int(pt_data[4]);
	} if(NUM_PTS > 5){
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("F4: ");
		m_usb_tx_int(pt_data[5]);
	} if(NUM_PTS > 6) {
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("F5: ");
		m_usb_tx_int(pt_data[6]);
	} if(NUM_PTS > 7) {
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("F6: ");
		m_usb_tx_int(pt_data[7]);
	} if(NUM_PTS > 8) {
		m_usb_tx_string("   ---   ");
		m_usb_tx_string("F7: ");
		m_usb_tx_int(pt_data[8]);
	}
	m_usb_tx_string("   ---   ");
	//m_usb_tx_char(13);
}
