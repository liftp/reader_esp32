#include "Menu.h"
#include "SDModel.h"
#include "Screen.h"

void display();
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

// 显示相关
// 最大菜单数，TODO暂时直接定义，后面可以修改为根据屏幕高度自适应
uint8_t max_menus_line = 5;
// 滚动列表（菜单/书籍）
ScrollMenu scroll = {
    .max_line_num = max_menus_line,
    .all_len = -1,
    .data_start_ptr = NULL,
    .curr_ptr = NULL,
    .topmost = NULL,
    .lowest = NULL
};

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
    .display = &display_books,
    // .enter_call = &enter_menu,
    .enter_call = NULL,
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


void init_menu() {
    menu();
    home -> display();
}

/**
 * 初始化滚动位置
*/
void init_scroll() {
    FileInfo *f_info = file_list;
    FileInfo *last_info = NULL;
    if (f_info == NULL) {
        Serial.println("file list is empty");
        scroll.all_len = 0;
        return;
    }
    scroll.topmost = f_info;
    scroll.data_start_ptr = f_info;
    scroll.curr_ptr = f_info;
    // 链表总长
    int all_len = 0;
    do {
        // 标记最底位置指针
        if (scroll.max_line_num == all_len) {
            scroll.lowest = f_info;
        }
        all_len ++;
        // 记录链表前一个位置
        last_info = f_info;
        // 指针下移
        f_info = f_info->next;
    } while (f_info != file_list && f_info != NULL);
    // 有可能文件少于最大显示行数,最底部指针指向末尾
    if (all_len < scroll.max_line_num) {
        scroll.lowest = last_info;
    }
    scroll.all_len = all_len;
}


/**
 * 书籍列表展示
*/
void display_books() {
    curr_m = &m_book_list;
    if (scroll.all_len == -1) {
        init_scroll();
    }

    // 调用屏幕显示
    const char **str_arr = (const char**)malloc(sizeof(char*)*max_menus_line);
    int i = 0;
    // 空的不显示
    if (!file_list) {
        return;
    }
    // 取链表中topmost -> lowest数据
    FileInfo *info = (FileInfo*) scroll.topmost;
    FileInfo *last = NULL;
    // 当前选择行内容，结束需要释放
    char* line = NULL;
    do {
        // Serial.println(info->name);
        // 加上当前选择标志
        if (info == scroll.curr_ptr) {
            int line_len = strlen(info->name) + 4;
            line = (char*)malloc(sizeof(char)*(line_len));
            memset(line, '\0', line_len);
            strcat(line, "> ");
            strcat(line, info->name);
            str_arr[i++] = line;
        } else {
            str_arr[i++] = info->name;
        }
        last = info;
        info = info -> next;
    } while (last != scroll.lowest);
    
    // 多行调用屏幕显示
    multi_line_menu_show(str_arr, i);

    // clear memory
    if (line != NULL) {
        free(line);
    }
    delete[] str_arr;
    
}

/**
 * 菜单直接展示
*/
void display() {
    if (!curr_m->display || !curr_m->next_menus) {
        return;
    }
    int len = curr_m->next_menus_len;
    const char *show_arr[max_menus_line] = {};
    char *curr_str = NULL;
    for (int i = 0; i < len; i++) {
        // show_arr[i] = 
        if (menu_pos == i) {
            curr_str = malloc_and_concat("> ", curr_m->next_menus[i] -> name.c_str(), NULL);
            show_arr[i] = curr_str;
        } else {
            show_arr[i] = curr_m->next_menus[i] -> name.c_str();
        }
        // Serial.println(action->name);
        // Serial.println((menu_pos == i ? "> " : "") + curr_m->next_menus[i] -> name);
    }

    free(curr_str);
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
    display();
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
    display();
    return {
        .name = curr_m->name,
        .attach = "",
        .pos = menu_pos,
        .is_menu = true
    };
}

/**
 * 下移书籍选择
*/
ChooseContent next_book(void) {
    ChooseContent choose = {
        .name = curr_m->name,
        .attach = "",
        .pos = menu_pos,
        .is_menu = false
    };
    // 书籍向下选择
    FileInfo *curr_file = (FileInfo*)scroll.curr_ptr;
    
    // 已经到了最底部
    if (curr_file == scroll.lowest) {
        // 文件数大于最大行，并且当前到了底部，将顶部和底部进行下移
        if (scroll.all_len > scroll.max_line_num) {
            scroll.lowest = ((FileInfo*) scroll.lowest)->next;
            scroll.topmost = ((FileInfo*) scroll.topmost)->next;
        }
    }
    scroll.curr_ptr = curr_file->next;
    Serial.println("curr_ptr:");
    Serial.println(((FileInfo*)scroll.curr_ptr) -> name);
    display_books();
    return choose;
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
    display();
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

