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
#include "robockey_robot.h"

#define NUM_FILTER_MEASUREMENTS 7	// change this if you want

// Takes in an empty (or not, but it will be overwritten) median_robot and an array of
// Robots past_n_msmnts and edits median_robot struct fields to be proper median values.
void median_filter(Robot* median_robot, Robot* past_n_msmnts[]);

#endif