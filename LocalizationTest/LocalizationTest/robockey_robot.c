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

#define RINK_SCALE 37.4358	// after measurement is scaled down to unit size by dividing out centroid
								// size, multiply by RINK_SCALE to change to cm for comparing to rink

#define PI 3.14159265359


/* Interpret the measurement vector returned by mWii and update Robot struct data */
void interpret(Robot* robot, unsigned int* measurement){
	int const RINK_SIZE[2] = {230, 120};	// cm
	float const CENTERED_STARS[4][2] = {{-0.2730, -17.3060}, {11.3820, 5.9350}, {-0.2730, 11.6940}, {-10.8360, -0.3230}}; // cm
	float const CENTERED_RATIOS[4] = {.328537, .243655, .222032, .205776}; // Ratios of distances of each star from center
	float const STAR_TRANSLATION[2] = {0.2730, 2.8060};	// cm

	// extract x and y data from measurement
	float mx[] = { measurement[0], measurement[3], measurement[6], measurement[9] };
	float my[] = { measurement[1], measurement[4], measurement[7], measurement[10] };
	
 /* FIND LOCATION (x,y) */
	// find centroid
	int i;
	float centroid[2] = {0, 0};
	for(i=0; i<4; i++){
		centroid[0] += mx[i]/4.0;	// in pixels
		centroid[1] += my[i]/4.0;	// in pixels
	}

	// find scale to get to unit size
	// scale = centroid size (sqrt of sum of distances^2 of points from centroid)
	float measurement_scale = 0;
	for(i=0;i<4;i++){
		measurement_scale += pow(mx[i]-centroid[0], 2) + pow(my[i]-centroid[1],2);
	}
	measurement_scale = pow(measurement_scale/4.0, 0.5);	// divide measurement by this to get unit size
	
	// convert centroids to cm
	centroid[0] *= RINK_SCALE/measurement_scale;	// now in cm
	centroid[1] *= RINK_SCALE/measurement_scale;
	
	// centroid position = robot position
	robot->x = centroid[0] + STAR_TRANSLATION[0];
	robot->y = centroid[1] + STAR_TRANSLATION[1];
	
	/* FIND ROTATION ANGLE */ 
	// center and scale star points
	for(i=0;i<4;i++){
		mx[i] *= RINK_SCALE/measurement_scale;
		my[i] *= RINK_SCALE/measurement_scale;
		mx[i] -= centroid[0];
		my[i] -= centroid[1];
	}
	
	// Find corresponding points based on scale of distance from center over total distance
	//float rd[4];
	//int indices[4] = {0, 1, 2, 3};
	//float total_distance = 0;

	//for (int i = 0; i < 4; i++) {
		//rd[i] = pow(pow(mx[i], 2) + pow(my[i], 2), 0.5);
		//total_distance += rd[i];
	//}
	//total_distance = pow(total_distance, 0.5);

	//for (int i = 0; i < 4; i++) {
		//rd[i] /= total_distance;
	//}

	// Sort ratios and get corresponding indices
	//for (int i = 0; i < 4; i++) {
		//for (int j = i + 1; j < 4; j++) {
			// If greater, swap
			//if (rd[j] > rd[i]) {
			//	int temp = rd[i], temp_i = indices[i];
			//	rd[i] = rd[j];
			//	indices[i] = indices[j];
			//	rd[j] = temp;
			//	indices[j] = temp_i;
			//}
		//}
	//}

	// angle = atan(sum(w*y - z*x) / sum(w*x + z*y))
	// (x, y) are the centered star points
	// (w, z) are the centered, scaled measurement points
	float num = 0;
	float den = 0;
	for (int i = 0; i < 4; i++) {
		num += CENTERED_STARS[i][0]*my[i] - CENTERED_STARS[i][1]*mx[i];
		den += CENTERED_STARS[i][0]*mx[i] + CENTERED_STARS[i][1]*my[i];
	}
	robot->o = atan2(den/num); // Updated angle in radians
}







		
		