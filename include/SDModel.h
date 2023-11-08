#ifndef SDMODEL_H
#define SDMODEL_H


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Common.h"
#include "driver/spi_master.h"

typedef struct FileInfo {
    const char *name;
    size_t size;
    FileInfo *next;
} FileType;

extern FileInfo *file_list;

void sd_spi_and_setup(SPIClass *spi);
File sd_file_write_ready(String);
bool sd_file_exists(String);
void sd_files_dir(String);
void freeFilesInfo();
#endif