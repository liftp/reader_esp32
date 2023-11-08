#include <Arduino.h>
#include <EEPROM.h>

// EEPROM.begin(1024);

void write_eep(int32_t val) {
    EEPROM.begin(1024);
    EEPROM.write(0, 1);
    EEPROM.writeLong(0, val);
    EEPROM.commit();
}

uint32_t read_eep() {
    // EEPROM.begin(1024);
    return EEPROM.readLong(0);
}