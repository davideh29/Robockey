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
#include "float.h"
#include "m_usb.h"
#include "score.h"

#define PI 3.14159265
#define CENTER_OFFSET_X -52
#define CENTER_OFFSET_Y 211

int const RINK_SIZE[2] = {230, 120};	// cm
float const CENTERED_STARS[4][2] = {{-0.2730, 11.6940}, {-0.2730, -17.3060}, {-10.8360, -0.3230}, {11.3820, 5.9350}}; // cm
float const STAR_TRANSLATION[2] = {0.2730, 2.8060};	// cm

// Prints mWii data
void print_data(float* mx, float* my, float ox, float oy, int top_index, int bottom_index, Robot* robot) {
	if (OPPONENT_X == robot->x) robot->x += 1;
	if (OPPONENT_Y == robot->y) robot->y += 1;
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
	m_usb_tx_string("Top Index: ");
	m_usb_tx_int(top_index);
	m_usb_tx_string("    ------    ");
	m_usb_tx_string("Bottom Index: ");
	m_usb_tx_int(bottom_index);
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
	m_usb_tx_int((int) (robot->o * 180.0) / PI);	// print angle in degrees
	m_usb_tx_string("   ---   Rho: ");
	m_usb_tx_int((int) ((180.0 * atan2f((OPPONENT_Y - robot->y), OPPONENT_X - robot->x)) / PI));
	m_usb_tx_char(13);
	m_usb_tx_char(13);
}

void init_robot(Robot* robot){
	robot->x = 0;
	robot->y = 0;
	robot->o = 0;
	robot->has_puck = false;
	// read in switch position to get color
	active = false;
}

void set_color(int robot_color){
	color = robot_color;
}

void play(){
	active = true;
}

void pause(){
	active = false;
}

/* Interpret the measurement vector returned by mWii and update Robot struct data */
void interpret(Robot* robot, unsigned int* measurement){
	// Star coordinates centered around the origin in order of distance from origin
	float const CENTERED_STARS[4][2] = {{-0.2730, -17.3060}, {11.3820, 5.9350}, {-0.2730, 11.6940}, {-10.8360, -0.3230}}; // cm

	// Extract x and y data from measurement
	float mWii_x[] = { (float) (measurement[0]-512.0), (float) (measurement[3]-512.0), (float) (measurement[6]-512.0), (float) (measurement[9]-512.0) };
	// Flipping y to right-handed coordinate system
	float mWii_y[] = { (float) -(measurement[1]-384.0), (float) -(measurement[4]-384.0), (float) -(measurement[7]-384.0), (float) -(measurement[10]-384.0) };

	
	////////// Determine translation to origin //////////
	float translation_x = 0, translation_y = 0;
	for (int i = 0; i < 4; i++) {
		translation_x += mWii_x[i];
		translation_y += mWii_y[i];
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
	float mx[4], my[4];
	for (int i = 0; i < 4; i++) {
		mx[i] = (mWii_x[i] - translation_x) / mWii_scale;
		my[i] = (mWii_y[i] - translation_y) / mWii_scale;
	}

	
	////////// Sort mWii point indices based on distance from origin //////////

	// Get max distance indices
	float max_distance = 0;
	int indices[2] = {0, 1};
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 4; j++) {
			// Get distance between points
			float distance = powf(powf(mx[i] - mx[j], 2.0) + powf(my[i] - my[j], 2.0), 0.5);
			// Compare to max distance
			if (distance > max_distance) {
				// If greater, update max distance indices
				indices[0] = i;
				indices[1] = j;
				max_distance = distance;
			}
		}
	}

	// Find which max index has the shortest distance to one of the other two points
	float min_distance = FLT_MAX;
	// Index of top star that we're looking for
	int top_index = indices[0];
	for (int i = 0; i < 4; i++) {
		// Looking at other two points
		if (i != indices[0] && i != indices[1]) {
			// Get distance to first max distance point
			float distance = powf(powf(mx[indices[0]] - mx[i], 2.0) + powf(my[indices[0]] - my[i], 2.0), 0.5);
			min_distance = distance;

			// Get distance to other max distance point
			distance = powf(powf(mx[indices[1]] - mx[i], 2.0) + powf(my[indices[1]] - my[i], 2.0), 0.5);
			// If distance is lower than min distance, set as the new top index
			if (distance < min_distance) {
				top_index = indices[1];
				min_distance = distance;
			}
			break;
		}
	}

	// Determine bottom star index
	int bottom_index;
	if (indices[0] == top_index) {
		bottom_index = indices[1];
	} else {
		bottom_index = indices[0];
	}

	////////// Determine mWii rotation //////////
	// determine rotation
	float delx = (mx[top_index] - mx[bottom_index]);
	float dely = (my[top_index] - my[bottom_index]);
	robot->o = atan2f(dely, delx); //find rotation from y axis


	////////// Use rotation to translate points into new xy frame //////////
	// Rotate original mWii points around the origin
	float s = sinf(2.0 * PI - robot->o);
	float c = cosf(2.0 * PI - robot->o);

	float ox = translation_x;
	float oy = translation_y;

	// Rotate points around origin into new xy frame
	translation_x = ox * c - oy * s;
	translation_y = oy * c + ox * s;

	// Set robot x and y locations in pixels
	robot->x = -translation_x - CENTER_OFFSET_X;
	robot->y = -translation_y - CENTER_OFFSET_Y;
	robot->o = -robot->o;

	// Print data
	print_data(mWii_x, mWii_y, ox, oy, top_index, bottom_index, robot);
}




		
		