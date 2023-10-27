#include "Menu.h"
#include "ButtonInterrupt.h"


void setup() {
    Serial.begin(9600);
    init_interr();
    // 菜单展示初始化
    intiDisplay();
}

void loop() {


    int key_flag = 0;
    if (key_flag = get_key()) {
        doAction(key_flag);
        reset_key();
    }
    delay(500);

}