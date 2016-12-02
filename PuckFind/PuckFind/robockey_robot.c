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

int const RINK_SIZE[2] = {230, 120};	// cm
float const CENTERED_STARS[4][2] = {{-0.2730, 11.6940}, {-0.2730, -17.3060}, {-10.8360, -0.3230}, {11.3820, 5.9350}}; // cm
float const STAR_TRANSLATION[2] = {0.2730, 2.8060};	// cm


/* Interpret the measurement vector returned by mWii and update Robot struct data */
void interpret(Robot* robot, unsigned int measurement[]){
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
	//	scale = centroid size (sqrt of sum of distances^2 of points from centroid)
	float measurement_scale = 0;
	for(i=0;i<4;i++){
		measurement_scale += pow(mx[i]-centroid[0],2) + pow(my[i]-centroid[1],2);
	}
	measurement_scale = pow(measurement_scale, 0.5);	// divide measurement by this to get unit size
	
	// convert centroids to cm
	centroid[0] *= RINK_SCALE/measurement_scale;	// now in cm
	centroid[1] *= RINK_SCALE/measurement_scale;
	
	// centroid position = robot position
	robot->x = centroid[0];
	robot->y = centroid[1];
	
	
 /* FIND ROTATION ANGLE */ 
	// center and scale star points
	for(i=0;i<4;i++){
		mx[i] *= RINK_SCALE/measurement_scale;
		my[i] *= RINK_SCALE/measurement_scale;
		mx[i] -= centroid[0];
		my[i] -= centroid[1];
	}
	
	// angle = acot( sum(v*x - u*y) / sum(u*x + v*y) )
	//	where (x,y) = centered, scaled measurement points
	//	and (u,v) = centered star points
	float num = 0;
	float den = 0;
	for(i=0; i<4; i++){
		num += CENTERED_STARS[i][1]*mx[i] - CENTERED_STARS[i][0]*my[i];
		den += CENTERED_STARS[i][0]*mx[i] - CENTERED_STARS[i][1]*my[i];
	}
	// update angle
	robot->o = atan(den/num);
}







		
		