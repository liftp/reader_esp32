#include "Menu.h"

String display(String param, MenuAction **action, int len);
void display_m();
void enter_menu(void);
ChooseContent next_menu(void);
ChooseContent next_book(void);
void no_op(void);
void no_op_str(String param);
void back_menu(void);
void enter_book(void);


// 菜单相关
MenuAction *home;
MenuAction *curr_m;
int menu_pos = 0;
String param = "Home";

// String first_lev[] = {"书籍", "接收文件", "ip", "wifi"};
// String read[] = {"阅读", "删除"};
// String delete_op[] = {"确认", "取消"};

// 初始化成员变量
// home
MenuAction m_home = {
    .name = "Home",
    .next_menus = NULL,
    .next_menus_len = 4,
    .last_lev_menus = NULL,
    .last_menus_len = 0,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = &next_menu,
    .back_call = &no_op,
    .level = 0
};


// home next

MenuAction m_book_list = {
    "书籍",
    .next_menus = NULL,
    .next_menus_len = 2,
    .last_lev_menus = NULL,
    .last_menus_len = 1,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = &next_book,
    .back_call = &back_menu,
    .level = 1
};

MenuAction m_rev_file = {
    "接收文件",
    .next_menus = NULL,
    .next_menus_len = 0,
    .last_lev_menus = NULL,
    .last_menus_len = 1,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 1
};

MenuAction m_ip = {
    "ip",
    .next_menus = NULL,
    .next_menus_len = 0,
    .last_lev_menus = NULL,
    .last_menus_len = 1,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 1
};

MenuAction m_wifi = {
    "wifi",
    .next_menus = NULL,
    .next_menus_len = 0,
    .last_lev_menus = NULL,
    .last_menus_len = 1,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 1
};

// book next
MenuAction m_read = {
    .name = "阅读",
    .next_menus = NULL,
    .next_menus_len = 0,
    .last_lev_menus = NULL,
    .last_menus_len = 4,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 2
};

MenuAction m_del = {
    .name = "删除",
    .next_menus = NULL,
    .next_menus_len = 2,
    .last_lev_menus = NULL,
    .last_menus_len = 4,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 2
};

// delete func next
MenuAction m_confirm = {
    .name = "确认",
    .next_menus = NULL,
    .next_menus_len = 0,
    .last_lev_menus = NULL,
    .last_menus_len = 2,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 3
};

MenuAction m_cancel = {
    .name = "取消",
    .next_menus = NULL,
    .next_menus_len = 0,
    .last_lev_menus = NULL,
    .last_menus_len = 2,
    .param_val = "",
    .display = &display,
    .enter_call = &enter_menu,
    .choose_call = NULL,
    .back_call = &back_menu,
    .level = 3
};

MenuAction* home_m_arr[] = {&m_home};
MenuAction* first_lev_menu[] = {&m_book_list, &m_rev_file, &m_ip, &m_wifi};
MenuAction* book_next_ms[] = {&m_read, &m_del};
MenuAction* del_next_ms[] = {&m_confirm, &m_cancel};


void intiDisplay() {
    menu();
    param = home -> display("", home->next_menus, home->next_menus_len);
}

String display(String param, MenuAction **action, int len) {
    Serial.println("\n=======");
    for (int i = 0; i < len; i++) {
        // Serial.println(action->name);
        Serial.println((menu_pos == i ? "> " : "") + action[i] -> name);
    }
    Serial.println("=======");
    return "";
}

void display_m() {
    Serial.println(">>>>" + curr_m->name + "<<<<");
    if (curr_m->display && curr_m->next_menus) {
        // 刷新显示
        curr_m->display("", curr_m->next_menus, curr_m->next_menus_len);
    }
}

void enter_menu(void) {
    if (!curr_m->next_menus) {
        return;
    }
    // 切换下级菜单
    curr_m = (curr_m->next_menus)[menu_pos];
    // 光标切换到下级菜单0位置
    menu_pos = 0;
    // 刷新显示
    display_m();
}

/**
 * 进入书籍目录
*/
void enter_book(void) {
    if (!curr_m->next_menus) {
        return;
    }
    // 切换下级菜单
    curr_m = (curr_m->next_menus)[menu_pos];
    // 光标切换到下级菜单0位置
    menu_pos = 0;
    // TODO 显示sd根目录书籍目录
}

ChooseContent next_menu(void) {
    if (!curr_m -> next_menus) {
        return {"", "", 0, true};
    }
    menu_pos = (menu_pos + 1) % curr_m->next_menus_len;
    display_m();
    return {
        .name = curr_m->name,
        .attach = "",
        .pos = menu_pos,
        .is_menu = true
    };
}

ChooseContent next_book(void) {
    if (!curr_m->next_menus) {
        return {"", "", 0, true};
    }
    menu_pos = (menu_pos + 1) % curr_m->next_menus_len;
    return {
        .name = curr_m->name,
        .attach = "",
        .pos = menu_pos,
        .is_menu = false
    };
}

/**
 * 返回上级菜单
*/
void back_menu(void) {
    if (!curr_m->last_lev_menus) {
        // Serial.println("no last menus");
        return;
    }
    curr_m = (curr_m->last_lev_menus)[0];
    menu_pos = 0;
    display_m();
}

void no_op(void) {

}

void no_op_str(String param) {

}

    

/**
 * 菜单关系绑定
*/
void menu() {

    // home 设置
    home = &m_home;
    // 当前菜单
    curr_m = home;

    // home -> next
    m_home.next_menus = first_lev_menu;

    // home (home) <- next
    m_book_list.last_lev_menus = home_m_arr;
    m_rev_file.last_lev_menus = home_m_arr;
    m_ip.last_lev_menus = home_m_arr;
    m_wifi.last_lev_menus = home_m_arr;

    // book -> next
    m_book_list.next_menus = book_next_ms;
    
    // book (first_lev_menu) <- next
    m_del.last_lev_menus = first_lev_menu;
    m_read.last_lev_menus = first_lev_menu;

    // del -> next
    m_del.next_menus = del_next_ms;

    // del (book_next_ms) <- next
    m_confirm.last_lev_menus = book_next_ms;
    m_cancel.last_lev_menus = book_next_ms;

}

void doAction(int action) {
        Serial.println(action);
    // 中断左、中、右
        if (action == MID_ACTION) {
            // 下一个菜单
            if (curr_m->choose_call) {
                // 菜单目标
                curr_m->choose_call();
            }
        } else if (action == LEFT_ACTION) {
            if (curr_m->back_call) {
                curr_m -> back_call();
            }
            
        } else if (action == RIGHT_ACTION) {
            if (curr_m->enter_call) {
                curr_m -> enter_call();
            }
        }
}

