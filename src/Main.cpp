#include "Menu.h"
#include "ButtonInterrupt.h"
#include "SDModel.h"
#include "Screen.h"

#include "ReaderPosRecord.h"

SPIClass spi = SPIClass(VSPI);

void setup() {
    // 初始化spi和sd卡对应的书籍列表
    sd_spi_and_setup(&spi);
    // 初始化屏幕相关模块
    init_screen(spi);
    // 初始化菜单,及home菜单展示
    init_menu();
    // 初始化中断
    init_interr();
}

void loop() {


    // 中断控制菜单
    int key_flag = 0;
    if (key_flag = get_key()) {
        doAction(key_flag);
        reset_key();
    }
    delay(500);


}