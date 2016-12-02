/* Code by Andrea Frank (2016) */

/* MEDIAN FILTER
In case I don't get the particle filter working, here's median filter methods. All
it does is take in the past NUM_SAVED_DATA measurements and spit out the median
position and orientation to reduce noise.
*/

#include "robockey_median_filter.h"

Robot past_data[NUM_SAVED_DATA];
bool past_data_initialized = false;

void initialize_past_data(){
	int i;
	for(i=0;i<NUM_SAVED_DATA;i++){
		past_data[i].x = 0;
		past_data[i].y = 0;
		past_data[i].o = 0;
	}
	past_data_initialized = true;
}

// compare function
int compare_function(const void * a, const void * b){
	return (int)( *(float*)a - *(float*)b );
}

void update_past_data(Robot* new_data){
	int i;
	for(i=NUM_SAVED_DATA; i>0; i--){
		past_data[i] = past_data[i-1];
	}
	past_data[0] = *new_data;
}

// takes in new measurement data, updates past_data array, and updates "new data"
// to median filtered new data
void median_filter(Robot* new_data){
	// if past_data was not initialized, initialize it
	if(!past_data_initialized){
		initialize_past_data();
	}
	
	// update past_data array
	update_past_data(new_data);
	
	// separate x, y, and o into their own lists
	int i;
	float xlist[NUM_SAVED_DATA];
	float ylist[NUM_SAVED_DATA];
	float olist[NUM_SAVED_DATA];
	for(i=0; i<NUM_SAVED_DATA; i++){
		xlist[i] = past_data[i].x;
		ylist[i] = past_data[i].y;
		olist[i] = past_data[i].o;
	}
	
	// sort each list
	qsort(xlist, NUM_SAVED_DATA, sizeof(float), compare_function);
	qsort(ylist, NUM_SAVED_DATA, sizeof(float), compare_function);
	qsort(olist, NUM_SAVED_DATA, sizeof(float), compare_function);
	
	// find median of each list
	int med_index = NUM_SAVED_DATA/2;
	float x_med = xlist[med_index];
	float y_med = ylist[med_index];
	float o_med = olist[med_index];
	
	// edit new_data to have median measurements
	new_data->x = x_med;
	new_data->y = y_med;
	new_data->o = o_med;
}