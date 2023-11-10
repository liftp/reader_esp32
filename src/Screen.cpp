#include "Screen.h"



#define S_SCK 18
#define DC 14
#define S_MOSI 23 
#define S_MISO 19
#define BUSY 15
#define RST 2
#define S_SS 5

#define MAX_LINE_CN 17

// ESP32 SS=5,SCL(SCK)=18,SDA(MOSI)=23,BUSY=15,RST=2,DC=27

char* get_line_content(const char *str, uint16_t start_pos);


#define USE_HSPI_FOR_EPD
// 2.13'' EPD Module
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(/*CS=5*/ S_SS, /*DC=*/ DC, /*RST=*/ RST, /*BUSY=*/ BUSY)); // DEPG0213BN 122x250, SSD1680
// GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEY0213Z98 122x250, SSD1680

extern const uint8_t chinese_gb2312[253023] U8G2_FONT_SECTION("chinese_gb2312");

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
// 2.9'' EPD Module
//GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // DEPG0290BS 128x296, SSD1680
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029C90 128x296, SSD1680
uint8_t cn_char_len = 0;
uint8_t en_char_len = 0;
uint16_t screen_w = 0;

void refresh_screen() {
  do {

  } while (display.nextPage());
}

void init_char_width() {
  cn_char_len = u8g2Fonts.getUTF8Width("一") + 1;
  // 实际i会比a窄
  en_char_len = u8g2Fonts.getUTF8Width("a") + 1;
}

/**
 * 指定到某行显示操作菜单
*/
void line_pos_show_menu(const char **str, int line_size, int show_line) {
  uint16_t left_x = 15;
  uint16_t init_height = 15;
  uint16_t h_gap = 5;
  int box_h = line_size * init_height + ((line_size - 1) * h_gap);
  // TODO 菜单宽暂时固定,应该根据str的字符串最宽决定
  int box_w = 4 * cn_char_len + 10;
  // 当前行的中间
  int box_y = show_line * init_height + ((show_line - 1) * h_gap) - (init_height / 2);
  if (box_y < 0)  {
    // 防止初始高度超过屏幕顶部
    box_y = 0;
  } else if (box_y + box_h > display.height()) {
    // 防止底部高度超过屏幕底部，但仍需注意是否偏移到屏幕顶
    box_y -= (box_y + box_h - display.height());
    box_y = box_y > 0 ? box_y : 0;
  }
  // TODO 屏幕水平位置暂未兼容超出屏幕情况
  int box_x = display.width() / 2;
  display.setPartialWindow(box_x, box_y, box_w, box_h);
  display.firstPage();
  int i = 0;
  // 局部填白
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  
  while (i < line_size) {
    // 设置文字
    u8g2Fonts.setCursor(box_x + 5, box_h + 2);
    u8g2Fonts.print(str[i++]);
    // 下划线
    display.drawFastHLine(box_x, box_h + 3, box_w, GxEPD_BLACK);
  }
  refresh_screen();
}

/**
 * 菜单显示
 * @param str 多行数据数组指针
 * @param line_num 显示行数
*/
void multi_line_menu_show(const char **str, int line_num) {
  display.setFullWindow();
  display.firstPage();
  uint16_t left_x = 15;
  uint16_t init_height = 15;
  uint16_t h_gap = 5;
  Serial.println(line_num);

  int i = 0;
  while (i < line_num) {
    u8g2Fonts.setCursor(left_x, init_height);
    init_height += cn_char_len + h_gap;
    u8g2Fonts.println(str[i++]);
  }
  // 刷新屏幕
  refresh_screen();
}
/**
 * 文本内容多行展示到屏幕
*/
uint16_t text_multi_line_show(const char *str) {
  
  screen_w = display.width();
  uint16_t str_len = strlen(str);
  // 每一行结尾在str的下标
  uint16_t get_pos = 0;
  // 第一行的行高
  uint16_t init_height = 15;
  // 记录行数
  uint8_t lines_num = 0;
  // 行间距
  uint8_t line_gap = 2;
  
  display.setFullWindow();
  display.firstPage();

  // 获取到全部内容或者显示高度超出屏幕退出
  while (get_pos < str_len - 1 && init_height <= display.height()) {
    char *line = get_line_content(str, get_pos);
    u8g2Fonts.setCursor(0, init_height);
    u8g2Fonts.println(line);
    init_height += cn_char_len + line_gap;
    get_pos += strlen(line);
    lines_num ++;
    free(line);
  }
  // Serial.println(init_height);
  // Serial.println(display.height() - cn_char_len - 1);
  // Serial.println(lines_num);
   
  refresh_screen();
  return get_pos;
}

/**
 * 获取一行的内容，由于超出后会不显示，且中英文字符长度不同，所以特殊处理
*/
char* get_line_content(const char *str, uint16_t start_pos) {
  uint16_t str_len = strlen(str);
  uint8_t line_ch_len  = screen_w / en_char_len;
  // 字符串占总屏幕宽度,默认两边间距2
  uint16_t all_len = 2;
  uint16_t i = start_pos;
  while (i < strlen(str) - 1 && all_len <= (screen_w - cn_char_len)) {
    // 中文字符
    if (str[i] & 0x80) {
      i += 3;
      all_len += cn_char_len;
    } else {
      i += 1;
      all_len += en_char_len;
    }
  }

  uint8_t result_len = i - start_pos;
  char *result = (char*)malloc(sizeof(char)*(result_len+1));
  uint8_t j = 0;
  for (; j < result_len; j++) {
    result[j] = str[start_pos+j];
  }
  result[j] = '\0';
  // 回调显示，释放内存
  // call_back(result, 0, 0);
  // free(result);
  return result;
}

void init_screen(SPIClass &hspi) {
  // hspi.begin(S_SCK, S_MISO, S_MOSI, S_SS);
  display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));
  display.init(115200,true,50,false);
  display.setRotation(1);
  u8g2Fonts.begin(display);
  // display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  u8g2Fonts.setFont(chinese_gb2312);
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFontDirection(0);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  init_char_width();
}
