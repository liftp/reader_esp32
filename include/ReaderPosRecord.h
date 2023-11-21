#ifndef READER_POS_RECORD_H
#define READER_POS_RECORD_H

#include <Arduino.h>
#include <EEPROM.h>

void write_eep(int32_t val);
uint32_t read_eep();
void init_eep();
void write_eep_curr(int32_t val);
void write_eep_next(int32_t val);
uint32_t read_eep_curr();
uint32_t read_eep_next();

#endif 