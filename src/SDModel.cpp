#include "SDModel.h"

#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 4

uint8_t buf[100] = {};
int file_pos = 0;
FileInfo *file_list;
/**
 * spi和sd启动
*/
void sd_spi_and_setup() {
    SPIClass spi = SPIClass(VSPI);
    spi.begin(SCK, MISO, MOSI, SS);

    if (!SD.begin(4)) {
        Serial.println("sd init failed");
    }

    Serial.println("sd init done.");

    // 读取目录所有文件
    sd_files_root();
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
 * 读取根目录所有文件，存到全局变量
*/
void sd_files_root() {
    File read = SD.open("/", FILE_READ);
    File next_file = read.openNextFile();
    FileInfo *last_file = NULL;
    if (next_file) {
        if (!next_file.isDirectory()) {
            FileInfo *file = (FileInfo*)malloc(sizeof(FileInfo));
            file -> name = next_file.name();
            file -> size = next_file.size();
            file-> next = NULL;
            if (last_file != NULL) {
                last_file->next = file;
            } else {
                // 文件链表头
                last_file = file;
                file_list = file;
            }
        } 
        next_file = read.openNextFile();
    }
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
    File file = SD.open(file_path, FILE_READ);
    file.seek(file_pos);
    int size = file.read(buf, 100);
    file.close();
    return String(buf, size);
}



