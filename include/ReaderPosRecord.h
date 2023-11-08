#ifndef READER_POS_RECORD_H
#define READER_POS_RECORD_H

#include <Arduino.h>
#include <EEPROM.h>

void write_eep(int32_t val);
uint32_t read_eep();

#endif 