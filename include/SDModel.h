#ifndef SDMODEL_H
#define SDMODEL_H


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

typedef struct FileInfo {
    String name;
    int size;
    FileInfo *next;
} FileType;

extern FileInfo *file_list;

void sd_spi_and_setup();
File sd_file_write_ready(String);
bool sd_file_exists(String);
void sd_files_root();
void freeFilesInfo();
#endif