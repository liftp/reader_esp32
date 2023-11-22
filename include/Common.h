#ifndef COMMON_H
#define COMMON_H
#include "Arduino.h"
// #include "stdlib.h"

#define LEFT_ACTION 1
#define MID_ACTION 2
#define RIGHT_ACTION 3


char* malloc_and_concat(const char* pre, const char* mid, const char* end);

int ends_with(const char *trg, const char* ends);
#endif