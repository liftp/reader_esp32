#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

#include "Arduino.h"

#define key1 25
#define key2 33
#define key3 32

// extern volatile int key_flag;

void init_interr();
void key_left_isr();
void key_mid_isr();
void key_right_isr();

int get_key();
void reset_key();
#endif