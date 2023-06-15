#include <cache/lru_cache.h>


void lru_add_block(uint64_t block_number, data_block_t* block)
{
  pthread_mutex_lock(&lru_cache->lock);
  // Find the oldest block in the cache
  int oldest_block_index = 0;
  for (int i = 0; i < lru_cache->size; ++i)
  {
    if (lru_cache->entries[i].block_number == block_number)
    {
      lru_cache->entries[i].block = block;
      lru_cache->entries[i].age = 0;
      pthread_mutex_unlock(&lru_cache->lock);
      return;
    }
    if (lru_cache->entries[i].age > lru_cache->entries[oldest_block_index].age)
    {
      oldest_block_index = i;
    }
  }
  if (lru_cache->size < LRU_CACHE_SIZE)
  {
    oldest_block_index = lru_cache->size;
    lru_cache->size++;
  }
  else
  {
    free(lru_cache->entries[oldest_block_index].block);
  }

  lru_cache->entries[oldest_block_index].block_number = block_number;
  lru_cache->entries[oldest_block_index].block = block;
  lru_cache->entries[oldest_block_index].age = 0;
  pthread_mutex_unlock(&lru_cache->lock);
}

data_block_t* lru_get_block(uint64_t block_number)
{
  pthread_mutex_lock(&lru_cache->lock);

  for (int i = 0; i < lru_cache->size; ++i)
  {
    if (lru_cache->entries[i].block_number == block_number)
    {
      pthread_mutex_unlock(&lru_cache->lock);
      lru_cache->entries[i].age = 0;
      return lru_cache->entries[i].block;
    }
  }

  pthread_mutex_unlock(&lru_cache->lock);
  return NULL;
}

void lru_increment_ages()
{
  pthread_mutex_lock(&lru_cache->lock);

  for (int i = 0; i < lru_cache->size; ++i)
  {
    lru_cache->entries[i].age++;
  }

  pthread_mutex_unlock(&lru_cache->lock);
}

// Function to initialize the LRU cache
void init_lru_cache()
{
  pthread_mutex_init(&lru_cache->lock, NULL);
  lru_cache->size = 0;
  for (int i = 0; i < LRU_CACHE_SIZE; ++i)
  {
    lru_cache->entries[i].block_number = 0;
    lru_cache->entries[i].block = NULL;
    lru_cache->entries[i].age = 0;
  }
}

// Path: src/fs_cache.c
