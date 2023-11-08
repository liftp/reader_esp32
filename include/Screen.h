#ifndef SCREEEN_H
#define SCREEEN_H

#define ENABLE_GxEPD2_GFX 0
#include "driver/spi_master.h"
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
// #include <Fonts/FreeMonoBold9pt7b.h>

// #include <U8g2lib.h>
#include <u8g2_wqy.h>
#include <U8g2_for_Adafruit_GFX.h>

void multi_line_menu_show(const char **str, int line_num);

void init_screen(SPIClass &spi);

#endif