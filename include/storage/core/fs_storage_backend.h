#ifndef FS_STORAGE_BACKEND_H
#define FS_STORAGE_BACKEND_H

#include <stdint.h>
#include "structures/fs_data_block.h"

typedef struct storage_backend_t
{
  void (*write_block)(uint64_t block_number, data_block_t* block);
  data_block_t* (*read_block)(uint64_t block_number);
} storage_backend_t;

storage_backend_t* create_backend(void (*write_block)(uint64_t, data_block_t*), data_block_t* (*read_block)(uint64_t));
void set_storage_backend(storage_backend_t* backend);
void write_block_to_storage(uint64_t block_number, data_block_t* block);
data_block_t* read_block_from_storage(uint64_t block_number);

#endif // FS_STORAGE_BACKEND_H
