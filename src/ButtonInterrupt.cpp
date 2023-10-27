#include "ButtonInterrupt.h"

volatile int key_flag = 0;

void init_interr() {
    // 下拉电阻
    pinMode(key1, INPUT_PULLDOWN);
    pinMode(key2, INPUT_PULLDOWN);
    pinMode(key3, INPUT_PULLDOWN);

    // 下降沿触发
    attachInterrupt(digitalPinToInterrupt(key1), key_left_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(key2), key_mid_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(key3), key_right_isr, FALLING);
}


void key_left_isr() {
    key_flag = 1;
}
void key_mid_isr() {
    key_flag = 2;
}
void key_right_isr() {
    key_flag = 3;
}

int get_key() {
    return key_flag;
}

void reset_key() {
    key_flag = 0;
}