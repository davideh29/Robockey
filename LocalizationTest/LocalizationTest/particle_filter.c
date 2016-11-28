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

#include "particle_filter.h"

float sense_noise = 0.5;

/* helper methods */

float max_prob(float* probs[]){
	int i;
	float max = 0;
	for(i=0; i<NUM_PARTICLES; i++){
		if(max < *probs[i]) {
			max = *probs[i];
		}
	}
	return max;
}

int rand_int(int seed, int max_value){
	srand((int)&seed);
	int r = rand();
	r = r % max_value;
	return r;
}

float rand_float(int seed, float max_value){
	srand((int)&seed);
	int r = rand();
	float s = ((float)r / RAND_MAX * max_value);
	return s;
}

/* Main Particle Filter Methods */

// calculates how probable this position is
float measurement_prob(Robot* robot, Robot* particle){
	float prob = 1.0;
	prob *= GAUSSIAN(robot->x, sense_noise, particle->x);
	prob *= GAUSSIAN(robot->y, sense_noise, particle->y);
	prob *= GAUSSIAN(robot->o, sense_noise, particle->o);
	
	return prob;
}
		
Robot* resample(Robot* robot, Robot* particles[], float probs[]){
	/*TODO: implement resampling wheel
	Pseudocode: 1) beta = random (uniform dist) from 0 to 2*max_prob
					in particle sample
				2) start from a random particle's probability
				3) if beta > prob[i], beta = beta - prob[i]; i++;
				4) when beta < prob[i], particle[i] is added to new sample
	This makes it so every particle is resampled with likelihood
	proportional to its probability weight from measurement_prob method */
	
	/* index = int(random.random() * N)
	mw = max_prob(probs);
	float beta = rand() * 2.0 * mw;
    while beta > w[index]:
        beta -= w[index]
        index = (index + 1) % N
    p3.append(p[index]) */
	return particles[0];
}
