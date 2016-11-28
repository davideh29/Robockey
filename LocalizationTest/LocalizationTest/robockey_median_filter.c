/* Code by Andrea Frank (2016) */

/* MEDIAN FILTER
In case I don't get the particle filter working, here's median filter methods. All
it does is take in the past NUM_FILTER_MEASUREMENTS measurements and spit out the median
position and orientation to reduce noise.
*/

#include "robockey_median_filter.h"

Robot past_n_measurements[NUM_FILTER_MEASUREMENTS];
bool past_n_measurements_initialized = false;

void initialize_past_n_measurements(){
	int i;
	for(i=0;i<NUM_FILTER_MEASUREMENTS;i++){
		past_n_measurements[i].x = 0;
		past_n_measurements[i].y = 0;
		past_n_measurements[i].o = 0;
	}
	past_n_measurements_initialized = true;
}

// compare function
int compare_function(const void * a, const void * b){
	return (int)( *(float*)a - *(float*)b );
}

// takes in an empty (or not, but it will be overwritten) median_robot and an array of
// Robots past_n_msmnts and edits median_robot struct fields to be proper median values
void median_filter(Robot* median_robot, Robot* past_n_msmnts[]){
	// if past_n_measurements was not initialized, initialize it
	if(!past_n_measurements_initialized){
		initialize_past_n_measurements();
	}
	
	// separate x, y, and o into their own lists
	int i;
	float xlist[NUM_FILTER_MEASUREMENTS];
	float ylist[NUM_FILTER_MEASUREMENTS];
	float olist[NUM_FILTER_MEASUREMENTS];
	for(i=0; i<NUM_FILTER_MEASUREMENTS; i++){
		xlist[i] = past_n_msmnts[i]->x;
		ylist[i] = past_n_msmnts[i]->y;
		olist[i] = past_n_msmnts[i]->o;
	}
	
	// sort each list
	qsort (xlist, NUM_FILTER_MEASUREMENTS, sizeof(float), compare_function);
	qsort (ylist, NUM_FILTER_MEASUREMENTS, sizeof(float), compare_function);
	qsort (olist, NUM_FILTER_MEASUREMENTS, sizeof(float), compare_function);
	
	// find median of each list
	int med_index = NUM_FILTER_MEASUREMENTS/2;
	float x_med = xlist[med_index];
	float y_med = ylist[med_index];
	float o_med = olist[med_index];
	
	// edit median_robot to have median measurements
	median_robot->x = x_med;
	median_robot->y = y_med;
	median_robot->o = o_med;
}