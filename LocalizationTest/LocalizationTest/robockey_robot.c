/* Code by Andrea Frank (2016) */

/* ROBOCKEY ROBOT
Defines Robot structure and allows interpretation of measurements into Robot
position and orientation for further motion planning. Basic measurement
interpretation algorithm is as follows:

Step 1: Extract x and y data from measurement.
	--- FIND LOCATION ---
Step 2:	Find centroid in pixels.	
Step 3: Determine scaling to get pixels to unit size (measurement_scale variable).
Step 4: Scale to cm using RINK_SCALE. This is your (x,y) location!
	--- FIND ORIENTATION ---
Step 5: Scale measured points to cm using measurement_scale from Step 3 and RINK_SCALE.
Step 6: Subtract out scaled centroid to center points.
Step 7: Calculate rotation angle using inverse trig.
*/

#include "robockey_robot.h"
#include "m_usb.h"
#define PI 3.14159265359
#define ANGLE_OFFSET 3.64

// Prints mWii data 
void print_data(float* mx, float* my, float ox, float oy, Robot* robot) {
	float opponent_x = -67.0, opponent_y = 248.0;
	// Print four points
	m_usb_tx_string(" --- ");
	for (int i = 0; i < 4; i++) {
		m_usb_tx_string("( ");
		m_usb_tx_int((int) mx[i]);
		m_usb_tx_string(", ");
		m_usb_tx_int((int) my[i]);
		m_usb_tx_string(" )");
		m_usb_tx_string("   ---   ");
	}
	// Print calculated translation and rotation
	m_usb_tx_char(13);
	m_usb_tx_string("   ---   Translation: ");
	m_usb_tx_string("( ");
	m_usb_tx_int((int) ox);
	m_usb_tx_string(", ");
	m_usb_tx_int((int) oy);
	m_usb_tx_string(" )");
	m_usb_tx_string("   ---   New frame: ");
	m_usb_tx_string("( ");
	m_usb_tx_int((int) robot->x);
	m_usb_tx_string(", ");
	m_usb_tx_int((int) robot->y);
	m_usb_tx_string(" )");
	m_usb_tx_string("   ---   Theta: ");
	m_usb_tx_int((int) (robot->o * 100.0));	// print angle in degrees
	m_usb_tx_string("   ---   Rho: ");
	m_usb_tx_int((int) (100.0 * (atan2f(robot->y - opponent_y, robot->x - opponent_x) + PI)));
	m_usb_tx_char(13);
	m_usb_tx_char(13);
}

/* Interpret the measurement vector returned by mWii and update Robot struct data */
void interpret(Robot* robot, unsigned int* measurement){
	// Star coordinates centered around the origin in order of distance from origin
	float const CENTERED_STARS[4][2] = {{-0.2730, -17.3060}, {11.3820, 5.9350}, {-0.2730, 11.6940}, {-10.8360, -0.3230}}; // cm

	// Extract x and y data from measurement
	float mWii_x[] = { (float) measurement[0], (float) measurement[3], (float) measurement[6], (float) measurement[9] };
	float mWii_y[] = { (float) measurement[1], (float) measurement[4], (float) measurement[7], (float) measurement[10] };

	
	////////// Determine translation to origin //////////
	float translation_x = 0, translation_y = 0;
	for (int i = 0; i < 4; i++) {
		translation_x += mWii_x[i] - 512.0;
		translation_y += mWii_y[i] - 384.0;
	}
	translation_x /= 4.0;
	translation_y /= 4.0;

	////////// Find mWii and star scale //////////
	float mWii_scale = 0.0, star_scale = 0.0;

	for (int i = 0; i < 4; i++) {
		mWii_scale += powf(mWii_x[i] - translation_x, 2.0) + powf(mWii_y[i] - translation_y, 2.0);
		star_scale += powf(CENTERED_STARS[i][0], 2.0) + powf(CENTERED_STARS[i][1], 2.0);
	}
	
	mWii_scale = powf(mWii_scale / 4.0, 0.5);
	star_scale = powf(star_scale / 4.0, 0.5);

	// Scale points to uniform scale (0 to 1)
	float sx[4], sy[4];
	float mx[4], my[4];
	for (int i = 0; i < 4; i++) {
		mx[i] = (mWii_x[i] - translation_x) / mWii_scale;
		my[i] = (mWii_y[i] - translation_y) / mWii_scale;
		sx[i] = CENTERED_STARS[i][0] / star_scale;
		sy[i] = CENTERED_STARS[i][1] / star_scale;
	}

	
	////////// Sort mWii point indices based on distance from origin //////////
	float rd[4];
	int indices[4] = {0, 1, 2, 3};

	// Get distances
	for (int i = 0; i < 4; i++) {
		rd[i] = powf(powf(mx[i], 2.0) + powf(my[i], 2.0), 0.5);
	}
	
	// Sort indices
	for (int i = 0; i < 3; i++) {
		int max = i;
		for (int j = i; j < 4; j++) {
			if (rd[j] > rd[max]) {
				max = j;
			}
		}
		// Swap array[i] and array[min]
		if (i != max) {
			// Update array order
			float temp = rd[i];
			rd[i] = rd[max];
			rd[max] = temp;
			// Update index order
			int tempIndex = indices[i];
			indices[i] = indices[max];
			indices[max] = tempIndex;
		}
	}
	

	////////// Determine mWii rotation //////////
	float num = 0;
	float den = 0;
	for (int i = 0; i < 4; i++) {
		num += sx[i]*my[indices[i]] - sy[i]*mx[indices[i]];
		den += sx[i]*mx[indices[i]] + sy[i]*my[indices[i]];
	}

	// Update robot orientation and account for angle offset
	robot->o = atan2(num, den) + PI - ANGLE_OFFSET; // Updated angle in radians
	if (robot->o < 0) robot->o += 2.0 * PI;


	////////// Use rotation to translate points into new xy frame //////////
	// Rotate original mWii points around the origin
	float s = sinf(2*PI-robot->o);
	float c = cosf(2*PI-robot->o);

	float ox = translation_x;
	float oy = translation_y;

	// Rotate points around origin into new xy frame
	//for (int i = 0; i < 4; i++) {
		translation_x = ox * c - oy * s;
		translation_y = oy * c + ox * s;
	//}

	// Set robot x and y locations in pixels
	robot->x = translation_x;
	robot->y = translation_y;

	// Print data
	print_data(mWii_x, mWii_y, ox, oy, robot);
}

// Scale
// Sorting
		
		