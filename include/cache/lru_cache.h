#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <structures/fs_data_block.h>
#include <fs_cache.h>

#define LRU_CACHE_SIZE 1024

typedef struct lru_cache_entry
{
  uint64_t block_number;
  data_block_t* block;
  uint64_t age;
} lru_cache_entry_t;

typedef struct lru_cache
{
  lru_cache_entry_t entries[LRU_CACHE_SIZE];
  uint64_t size;
  pthread_mutex_t lock;
} lru_cache_t;

static lru_cache_t* lru_cache;

void init_lru_cache();
void lru_add_block(uint64_t block_number, data_block_t* block);
data_block_t* lru_get_block(uint64_t block_number);
void lru_increment_ages();

#endif // LRU_CACHE_H
