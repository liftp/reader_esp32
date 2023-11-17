#ifndef SCREEEN_H
#define SCREEEN_H

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
// #include <Fonts/FreeMonoBold9pt7b.h>

// #include <U8g2lib.h>
#include <u8g2_wqy.h>
#include <U8g2_for_Adafruit_GFX.h>

void multi_line_menu_show(const char **str, int line_num);

void line_pos_show_menu(const char **str, int line_size, int show_line);

void init_screen(SPIClass &spi);

void center_tip(const char* tip);

uint16_t text_multi_line_show(const char *str);

uint16_t get_page_chars();


#endif