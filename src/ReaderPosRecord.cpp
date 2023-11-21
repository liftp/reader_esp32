#include <ReaderPosRecord.h>
/**
 * 初始化地址
*/
void init_eep() {
    EEPROM.begin(16);
}

void write_eep(int32_t val) {
    EEPROM.writeLong(0, val);
    EEPROM.commit();
}

uint32_t read_eep() {
    // EEPROM.begin(1024);
    return EEPROM.readLong(0);
}

void write_eep_curr(int32_t val) {
    EEPROM.writeLong(0, val);
    EEPROM.commit();
}

void write_eep_next(int32_t val) {
    EEPROM.writeLong(4, val);
    EEPROM.commit();
}

uint32_t read_eep_curr() {
    return EEPROM.readLong(0);
}

uint32_t read_eep_next() {
    return EEPROM.readLong(4);
}