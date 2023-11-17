#include "SDModel.h"


#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5    /**cs片选信号接4引脚*/


const char record_file[] = "/record/log.txt";
const char record_path[] = "/record";

// SPIClass spi = SPIClass(VSPI);


uint8_t buf[100] = {};
int file_pos = 0;
FileInfo *file_list;
const char *file_end_flag = "###FILE_EOF###";

void test_write() {
    File read;
    // unsigned char *str = (unsigned char*)"asd123fffww"; 
    File f = SD.open("/test.txt", FILE_WRITE);
    
    f.println("testasd");
    f.close();
    read = SD.open("/test.txt");
    Serial.println(read.readString());
    read.close();

    File f2 = SD.open("/test.txt", FILE_APPEND);
    f2.println("testasd");
    f2.close();

    read = SD.open("/test.txt");
    Serial.println(read.readString());
    read.close();

    File f3 = SD.open("/test.txt", FILE_WRITE);
    f3.println("testasd");
    Serial.println(f3.readString());
    f3.close();

    read = SD.open("/test.txt");
    Serial.println(read.readString());
    read.close();
}
/**
 * spi和sd启动
*/
void sd_spi_and_setup(SPIClass *spi) {
    spi -> begin(SCK, MISO, MOSI, SS);
    
    if (!SD.begin(SS, *spi)) {
        Serial.println("sd init failed");
        return;
    }
 
    // Serial.println("sd init done.");
    // 读取目录所有文件
    sd_files_dir("/");

}

/**
 * sd文件写入模式打开
*/
File sd_file_write_ready(String file_path) {
    return SD.open(file_path, FILE_WRITE);
}


bool sd_file_exists(String file_path) {
    return SD.exists(file_path);
}

/**
 * 字符串申请堆空间并拷贝
*/
char* str_melloc_cp(const char* trg) {
    int name_len = strlen(trg) + 1;
    char *name_str = (char *)malloc(sizeof(char) * name_len);
    memset(name_str, '\0', name_len);
    memcpy(name_str, trg, name_len - 1);
    return name_str;
}

/**
 * 读取目录下所有文件名称，存到全局变量
*/
void sd_files_dir(String dir) {
    File read = SD.open(dir);
    File next_file = read.openNextFile(FILE_READ);
    FileInfo *last_file = NULL;
    FileInfo *head = NULL;
    while (next_file) {
        bool dir_flag = next_file.isDirectory();
        Serial.print(dir_flag ? "dir ---" : "file---");
        Serial.println(next_file.name());
        // 文件且格式为txt
        if (!dir_flag && ends_with(next_file.name(), ".txt")) {
            FileInfo *file = (FileInfo*)malloc(sizeof(FileInfo));
            if (!file) {
                Serial.println("内存分配失败");
                return;
            }

            char *name_str = str_melloc_cp(next_file.name());
            file -> name = name_str;
            file -> size = next_file.size();
            file -> next = NULL;
            if (last_file != NULL) {
                last_file->next = file;
                last_file = last_file -> next;
            } else {
                last_file = file;
                // 文件链表头
                head = file;
            }
        }
        next_file.close();
        next_file = read.openNextFile(FILE_READ);
    }
    // 设置全局变量文件列表
    file_list = head;
}

/**
 * 释放内存
*/
void freeFilesInfo() {
    FileInfo *next = file_list;
    FileInfo *temp;
    while (next) {
        temp = next;
        next = next->next;
        free(temp);
    }
}

/**
 * sd从文件指定位置开始读
*/
String sd_read_file_in_pos(String file_path) {
    File file = SD.open(file_path);
    file.seek(file_pos);
    int size = file.read(buf, 100);
    file.close();
    return String(buf, size);
}

/**
 * 读取书籍文件记录的位置，如果没找到返回-1
*/
long book_recorder_read_pos(const char* filePath) {
    char end_flag = ';';
    char seperator = ':';
    if (SD.exists(record_file)) {
        File rec_file = SD.open(record_file, FILE_WRITE);
        rec_file.close();
    }
    File file = SD.open(record_file, FILE_READ);
    bool find = false;
    bool is_find = false;
    String line = file.readStringUntil(end_flag);
    line.trim();
    Serial.println(line);
    // 逐行读取内容直到找到对应内容
    while (is_find = !line.startsWith(filePath)) {
        if (!file.available()) {
            break;
        }
        line = file.readStringUntil(end_flag);
        line.trim();
        Serial.println(line);
    }
    long pos = -1;

    file.close();

    if (is_find) {
        // 读取位置
        int sep_index = line.indexOf(seperator);
        if (sep_index > 0) {
            String pos_str = line.substring(sep_index);
            pos_str.trim();
            pos = pos_str.toInt();
        }
    }
    return pos;
}

/**
 * 记录文件阅读位置，如果历史位置为-1,认为没有阅读，追加写尾部记录为0位置
*/
void record_book_read_pos(const char* file_path, long his_read_pos) {
    char end_flag = ';';
    // 追加写入该文件阅读进度为0位置
    if (his_read_pos == -1) {
        File file = SD.open(record_file, FILE_APPEND);
        String writeStr = file_path;
        writeStr.concat(":");
        writeStr.concat(his_read_pos);
        writeStr.concat(end_flag);
        file.println(writeStr);
        file.close();
    } else {
        // 整体读取，然后替换文件阅读进度,全文件覆写
        File file = SD.open(record_file, FILE_WRITE);
        String content = file.readString();
        int head_pos = content.indexOf(file_path);
        // 0 -> 匹配位置
        String writeStr = content.substring(0, head_pos);
        // 新的位置记录
        writeStr.concat(file_path);
        writeStr.concat(":");
        writeStr.concat(his_read_pos);
        writeStr.concat(end_flag);
        // 下一行位置 -> 文件尾部
        int next_line_pos = content.indexOf("\n", head_pos);
        if (next_line_pos < file.size()) {
            writeStr.concat(content.substring(next_line_pos + 1));
        }
        file.println(writeStr);
        file.close();
    }
}

/**
 * 从eep读取位置写入文件
*/
void record_book_read_pos_single(const char* file_name) {
    long pos = read_eep();
    char *rec_file = malloc_and_concat(record_path, "/", file_name);
    File f = SD.open(rec_file, FILE_WRITE);
    f.println(pos);
    f.close();
    free(rec_file);
}

void create_record_dir_if_not_exists() {
    if (!SD.exists(record_path)) {
        SD.mkdir(record_path);
    }
}
/**
 * 单文件读取阅读位置
*/
long book_recorder_read_pos_single(const char* file_name) {
    create_record_dir_if_not_exists();
    char *rec_file = malloc_and_concat(record_path, "/", file_name);
    if (!SD.exists(rec_file)) {
        File f = SD.open(rec_file, FILE_WRITE);
        f.println(0);
        f.close();
        return 0;
    }
    File f = SD.open(rec_file);
    String str = f.readString();
    str.trim();
    free(rec_file);
    f.close();
    return str.toInt();
}

/**
 * 读取位置并覆写eep
*/
long book_recorder_pos_and_write_eep(const char* file_name) {
    
    long pos =  book_recorder_read_pos_single(file_name);
    Serial.printf("file pos:%d\n", pos);
    write_eep(pos);
    return pos;
}



/**
 * 读取书籍的阅读记录位置，以及读出指定大小字符, 需要释放返回结果 free
*/
char* read_book_content_from_last_pos(const char* file_path, uint16_t read_size, long his_read_pos) {
    // 读取指定长度内容，可能文件结束只能读取最后剩余部分
    Serial.printf("open %s\n", file_path);
    File book = SD.open(file_path);
    
    book.seek(his_read_pos);
    char *buf = (char*)malloc(sizeof(char)*(read_size + 1));
    size_t real_read_pos = book.readBytes(buf, read_size);
    buf[real_read_pos] = '\0';
    book.close();
    return buf;
}
/**
 * 从指定位置往前读取字符，先读取指定数量字符，然后从尾部计算需要的字符数，并返回对应位置
*/
CharWithPos reverse_read_book_content_from_last_pos(const char* file_path, uint16_t read_size, long his_read_pos, uint16_t page_chars) {
    // 读取指定长度内容，可能文件结束只能读取最后剩余部分
    Serial.printf("open %s\n", file_path);
    File book = SD.open(file_path);
    // 需要记录下末尾位置
    long old_pos = his_read_pos;
    // 向前推字符数
    if (his_read_pos > read_size) {
        his_read_pos -= read_size;
    } else {
        his_read_pos = 0;
    }
    // 超出位置不进行读取
    if (his_read_pos >= 0 && his_read_pos <= book.size()) {
        book.seek(his_read_pos);
    } else {
        return {
            .str = NULL
        };
    }

    char *buf = (char*)malloc(sizeof(char)*(read_size + 1));
    size_t real_read_size = book.readBytes(buf, read_size);
    buf[real_read_size] = '\0';

    
    // 需要往前计算读取的字符数
    int i = real_read_size - 1;
    int sub = real_read_size - page_chars;
    // 当前位置是完整字符，往前找每个完整的字符，中文3字节，其他1字节，
    // 直到需要的字符数被找到，此位置作为读取内容的开始
    while (i > 3 && i > sub) {
        if (buf[i] & 0x80) {
            i -= 3;
        } else {
            i -= 1;
        }
    }


    book.close();
    CharWithPos result = {
        .str = buf,
        .start_pos = i
    };
    return result;
}



