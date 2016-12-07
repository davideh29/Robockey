/* Code by Andrea Frank (2016) */

/* MEDIAN FILTER
In case I don't get the particle filter working, here's median filter methods. All
it does is take in the past NUM_FILTER_MEASUREMENTS measurements and spit out the median
position and orientation to reduce noise.
*/

#ifndef robockey_median_filter_h
#define robockey_median_filter_h

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "robockey.h"

#define NUM_SAVED_DATA 7	// change this if you want
#define NUM_SAVED_DIRECTIONS 13 // DONT change this this is permanent

// Takes in an empty (or not, but it will be overwritten) median_robot
// and edits median_robot struct fields to be proper median values.
void filter_positions(Robot* median_data);

int filter_directions(int direction);

#endif