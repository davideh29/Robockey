
#ifndef robockey_wireless_communication_h
#define robockey_wireless_communication_h

#include "robockey.h"

bool has_message = false;

void init_rf(int robot_num);
void light_LED() ;
void handle_message();
void read_switch();

ISR(INT2_vect);

#endif