#include "Common.h"

/**
 * 前后字符串拼接，并返回生成新字符串，使用后需要手动释放内存
*/
char* malloc_and_concat(const char* pre, const char* mid, const char* end) {
    int pre_len = 0;
    int mid_len = 0;
    int end_len = 0;
    if (pre != NULL) {
        pre_len = strlen(pre);
    }
    if (mid != NULL) {
        mid_len = strlen(mid);
    }
    if (end != NULL) {
        end_len = strlen(end);
    }
    int all_len = pre_len + mid_len + end_len + 1;
    char *result = (char*)malloc(sizeof(char)*(all_len));
    memset(result, '\0', all_len);
    if (pre != NULL) {
        strcat(result, pre);
    }
    if (mid != NULL) {
        strcat(result, mid);
    }
    if (end != NULL) {
        strcat(result, end);
    }
    return result;
}

/**
 * 判断尾部字符串是否相等，返回0不同，1相同
*/
int ends_with(const char *trg, const char* ends) {
    int len = strlen(ends);
    int trg_len = strlen(trg);
    if (trg_len < len) {
        return 0;
    }

    return !strcmp(trg + trg_len - len, ends);
}