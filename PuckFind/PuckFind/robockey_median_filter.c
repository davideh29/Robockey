/* Code by Andrea Frank (2016) */

/* MEDIAN FILTER
In case I don't get the particle filter working, here's median filter methods. All
it does is take in the past NUM_SAVED_DATA measurements and spit out the median
position and orientation to reduce noise.
*/

#include "robockey_median_filter.h"

Robot past_data[NUM_SAVED_DATA];
int past_directions[NUM_SAVED_DIRECTIONS];
bool past_data_initialized = false;
bool past_directions_initialized;

void initialize_past_data(){
	int i;
	for(i=0;i<NUM_SAVED_DATA;i++){
		past_data[i].x = 0;
		past_data[i].y = 0;
		past_data[i].o = 0;
	}
	past_data_initialized = true;
}

void initialize_past_directions() {
	int i;
	for(i=0;i<NUM_SAVED_DIRECTIONS;i++){
		past_directions[i] = 0;
	}
	past_directions_initialized = true;
}

// compare function
int compare_function(const void * a, const void * b){
	return (int)( *(float*)a - *(float*)b );
}

// compare function
int compare_function_int(const void * a, const void * b){
	return *(int*)a - *(int*)b;
}

void update_past_data(Robot* new_data){
	int i;
	for(i=NUM_SAVED_DATA; i>0; i--){
		past_data[i] = past_data[i-1];
	}
	past_data[0] = *new_data;
}

void update_past_directions(int direction){
	int i;
	for(i=NUM_SAVED_DIRECTIONS; i>0; i--){
		past_directions[i] = past_directions[i-1];
	}
	past_directions[0] = direction;
}

// takes in new measurement data, updates past_data array, and updates "new data"
// to median filtered new data
void median_filter_positions(Robot* new_data){
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

// takes in new direction data, updates past_directions array, and updates "new direction"
// to median filtered new directions
int median_filter_directions(int direction){
	// if past_directions was not initialized, initialize it
	if(!past_directions_initialized){
		initialize_past_directions();
	}
	
	// update past_data array
	update_past_directions(direction);
	
	int temp[NUM_SAVED_DIRECTIONS];
	
	for (int i = 0; i < NUM_SAVED_DIRECTIONS; i++) {
		temp[i] = past_directions[i];
	}

	// sort each list
	qsort(temp, NUM_SAVED_DIRECTIONS, sizeof(int), compare_function_int);
	
	// find median of each list
	int med_index = NUM_SAVED_DIRECTIONS/2;
	int direction_med = temp[med_index];
	
	// return median direction
	return direction_med;
}