#include "robockey.h"

#ifndef match_commands_h
#define match_commands_h

void init_rf(int robot_num);
void light_LED();
void flash_LED();
void handle_message();
void read_switch();

#define CHANNEL 1

//ISR(INT2_vect);

#endif