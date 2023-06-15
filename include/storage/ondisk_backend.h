#ifndef DISK_STORAGE_BACKEND_H
#define DISK_STORAGE_BACKEND_H

#include "fs_storage_backend.h"


storage_backend_t* create_disk_backend(char* path);
static void disk_write_block(uint64_t block_number, data_block_t* block);
static data_block_t* disk_read_block(uint64_t block_number);


#endif // DISK_STORAGE_BACKEND_H
