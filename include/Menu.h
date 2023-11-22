#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <SD.h>
#include "Common.h"
#include "SDModel.h"

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

/**
 * 菜单结构说明：
 * MenuAction: 菜单及相关操作结构体
 *  name:名称
 *  **next_menus:关联下级菜单
 *  next_menus_len：下级菜单长度
 *  **last_lev_menus：关联上级菜单，用于返回回调
 *  last_menus_len:上级菜单长度
 *  
 *  简单三键功能
 *  void (*display):用于进入菜单后的直接展示
 *  void (*enter_call)：用于回调进入菜单，对应右键
 *  ChooseContent (*choose_call)：用于回调当前菜单的选择功能，对应中键，一般只能向下选择
 *  void (*back_call)：用于回调上级菜单功能，对应左键
*/

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
    void (*choose_call) ();
    // 左键
    void (*back_call) ();
    int level;
} *menuActionType;
/**
 * 适用于链表结构的滚动结构体
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
/**
 * 适用于数组结构的滚动结构体
*/
struct ScrollMenuArr {
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
void enter_read();
void next_page();
void last_page();
void exit_read();
#endif