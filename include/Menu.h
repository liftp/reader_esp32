#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <SD.h>
#include "Common.h"

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
    void (*display) ();
    // 右键
    void (*enter_call) ();
    // 中键
    ChooseContent (*choose_call) ();
    // 左键
    void (*back_call) ();
    int level;
} *menuActionType;
/**
 * 适用于环形链表结构的滚动结构体
*/
typedef struct ScrollMenu {
    // 最大行数
    int max_line_num;
    // 记录数据总长
    int all_len;
    // 开始位置，该指针不变
    void *data_start_ptr;
    // 当前指针，指示所选位置
    void *curr_ptr;
    // 最上位置指针
    void *topmost;
    // 最底位置指针
    void *lowest;
} *ScrollMenuPtr;

typedef struct ScrollMenuArr {
    // 最大行数
    int max_line_num;
    // 记录数据总长
    int all_len;
    int data_start_pos;
    int curr_pos;
    int topmost;
    int lowest;
};

void menu();
void init_menu();
void doAction(int action);
void display_books();
#endif