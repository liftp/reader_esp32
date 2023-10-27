#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <SD.h>

#define LEFT_ACTION 1
#define MID_ACTION 2
#define RIGHT_ACTION 3

typedef struct ChooseType {
    // 选择内容
    String name;
    // 附加信息
    String attach;
    // 当前列表位置
    int pos;
    // 是否菜单
    bool is_menu;

} ChooseContent;

typedef struct MenuAction {
    // 菜单名称
    String name;

    // 下级菜单列表
    MenuAction **next_menus;
    int next_menus_len;
    // struct MenuAction *next_lev_menus;
    // 上级菜单指针
    MenuAction **last_lev_menus;
    int last_menus_len;
    // 调用参数值
    String param_val;

    // 显示
    String (*display) (String param, MenuAction **action, int len);
    // 右键
    void (*enter_call) ();
    // 中键
    ChooseContent (*choose_call) ();
    // 左键
    void (*back_call) ();
    int level;
} *menuActionType;


void menu();
void intiDisplay();
void doAction(int action);
#endif