#include <fs_cache.h>


// Singleton instance of the cache
static cache_t* cache;

cache_t* create_cache(void (*add_block)(uint64_t, data_block_t*), data_block_t* (*get_block)(uint64_t))
{
  cache_t* new_cache = malloc(sizeof(cache_t));
  new_cache->add_block = add_block_to_cache;
  new_cache->get_block = get_block_from_cache;
  return new_cache;
}

void set_cache(cache_t* new_cache)
{
  cache = new_cache;
}

void add_block_to_cache(uint64_t block_number, data_block_t* block)
{
  cache->add_block(block_number, block);
}

data_block_t* get_block_from_cache(uint64_t block_number)
{
  return cache->get_block(block_number);
}
