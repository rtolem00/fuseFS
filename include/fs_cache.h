#ifndef FS_CACHE_H
#define FS_CACHE_H

#include <pthread.h>
#include <stdint.h>

#include "structures/fs_data_block.h"


typedef struct cache_entry_t
{
  uint64_t block_number;
  data_block_t* block;
} cache_entry_t;

typedef struct cache_t
{
  void (*add_block)(uint64_t, data_block_t*);
  data_block_t* (*get_block)(uint64_t);
} cache_t;


cache_t* create_cache(void (*add_block)(uint64_t, data_block_t*), data_block_t* (*get_block)(uint64_t));
void set_cache(cache_t* new_cache);
void add_block_to_cache(uint64_t block_number, data_block_t* block);
data_block_t* get_block_from_cache(uint64_t block_number);


#endif // FS_CACHE_H
