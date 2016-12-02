/* Code by Andrea Frank (2016) */

/* PARTICLE FILTER
The below is used for particle filtering - a method of multimodal, continuous Bayesian 
determination of the most likely position and orientation of the robot. The basic
algorithm is as follows:

Step 1: Make N (often N=1000) virtual robots ("particles") with randomized positions
			and orientations. Assume all have equal likelihood of being the real
			position and orientation.
Step 2: Read measurement from real robot (i.e. where the real robot thinks it is).
Step 3: Compare measurement with the virtual robots' position and orientation. Assign
			each virtual robot a probability based on its Z score with the actual
			measurement as the mean, and an estimated measurement std dev as sigma.
Step 4: Draw a new sample of N virtual robots from the old set using sample with
			replacement, with the likelihood of being added to the new set proportional
			to the probabilities assigned in Step 3.
Step 5: Repeat Steps 2-4. Virtual robots will converge in a small number of steps and
			you will have a very likely guess as to the robot's position and orientation.
	
This is the standard algorithm	used in modern robot localization problems
such as Google's Self-Driving Car. so, like, p neat.
*/
#ifndef particle_filter_h
#define particle_filter_h

#include <math.h>
#include <stdlib.h>
#include "robockey_robot.h"

#define NUM_PARTICLES 1000
#define PI 3.1415926535897
#define GAUSSIAN(mu, sigma, x) (exp(- (pow(mu - x, 2) / pow(sigma, 2) / 2.0) / sqrt(2.0 * PI * pow(sigma, 2))) )


 // calculates how probable this position is
float measurement_prob(Robot* robot, Robot* particle);
 
 // resample one particle based on 
Robot* resample(Robot* robot, Robot* particles[], float probs[]);

#endif
