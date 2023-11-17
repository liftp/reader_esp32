#ifndef SDMODEL_H
#define SDMODEL_H


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Common.h"
#include "driver/spi_master.h"
#include "ReaderPosRecord.h"

typedef struct FileInfo {
    const char *name;
    size_t size;
    FileInfo *next;
} FileType;

struct CharWithPos {
    char *str;
    size_t start_pos;
};

extern FileInfo *file_list;

void sd_spi_and_setup(SPIClass *spi);
File sd_file_write_ready(String);
bool sd_file_exists(String);
void sd_files_dir(String);
void freeFilesInfo();
void record_book_read_pos_single(const char* file_name);
long book_recorder_read_pos_single(const char* file_name);
long book_recorder_pos_and_write_eep(const char* file_name);
char* read_book_content_from_last_pos(const char* file_path, uint16_t read_size, long his_read_pos);
CharWithPos reverse_read_book_content_from_last_pos(const char* file_path, uint16_t read_size, long his_read_pos, uint16_t page_chars);
#endif