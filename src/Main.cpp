#include "Menu.h"
#include "ButtonInterrupt.h"
#include "SDModel.h"
#include "Screen.h"

#include "ReaderPosRecord.h"

#include "esp_sleep.h"

SPIClass spi = SPIClass(VSPI);

int operate_times = 0;
// 限制最大未操作次数 大约20s
int sleep_times = 40;

void setup() {
    Serial.begin(115200);
    // 初始化spi和sd卡对应的书籍列表
    sd_spi_and_setup(&spi);
    // 初始化屏幕相关模块
    init_screen(spi);
    // 初始化菜单,及home菜单展示
    init_menu();
    // 初始化中断
    init_interr();
    // 初始化eep
    init_eep();

    
}

void action() {
    // 中断控制菜单
    int key_flag = 0;
    if (key_flag = get_key()) {
        doAction(key_flag);
        reset_key();
        // 有操作重置操作次数
        operate_times = 0;
    }
    // 记录无操作时间，
    // operate_times ++;
    // 如果超过最大无操作次数，40*0.5s=20s，大约20s无操作；
    // 当然有可能被其他操作阻塞，例如文件传输，先不进行处理防止中断文件传输
    // if (operate_times > sleep_times) {
    //     // 休眠,33端口高电平恢复
    //     esp_sleep_enable_ext1_wakeup(1ull<<33, ESP_EXT1_WAKEUP_ANY_HIGH);
    //     esp_deep_sleep_start();
    //     operate_times = 0;
    // }
    delay(500);
}

void loop() {
    action();

}