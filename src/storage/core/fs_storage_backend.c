#include "fs_storage_backend.h"
#include "fs_cache.h"


// Singleton instance of the storage backend
static storage_backend_t* backend;

storage_backend_t* create_backend(void (*write_block)(uint64_t, data_block_t*), data_block_t* (*read_block)(uint64_t))
{
  storage_backend_t* new_backend = malloc(sizeof(storage_backend_t));
  new_backend->write_block = write_block;
  new_backend->read_block = read_block;
  return new_backend;
}

void set_storage_backend(storage_backend_t* new_backend)
{
  backend = new_backend;
}

void write_block_to_storage(uint64_t block_number, data_block_t* block)
{
  backend->write_block(block_number, block);
}

data_block_t* read_block_from_storage(uint64_t block_number)
{
  data_block_t* block = get_block_from_cache(block_number);

  if (block)
  {
    return block;
  }

  block = backend->read_block(block_number);

  if (block)
  {
    add_block_to_cache(block_number, block);
  }

  return block;
}

data_block_t* read_block_from_disk(uint64_t block_number)
{
    pthread_mutex_lock(&cache.lock);
    // First, try to get the block from the cache
    data_block_t* block = get_block_from_cache(block_number);
    pthread_mutex_unlock(&cache.lock);
    if (block) {
        return block;
    }

    // If the block is not in the cache, read it from disk
    char path[256];
    sprintf(path, "%s/block_%llu.bin", DATA_BLOCKS_DIR, block_number);

    FILE* file = fopen(path, "rb");
    if (!file) {
        // If the file doesn't exist, we assume the block is free and return NULL
        return NULL;
    }

    block = malloc(sizeof(data_block_t));
    fread(block, sizeof(data_block_t), 1, file);

    fclose(file);

    // Add the block to the cache
    add_block_to_cache(block_number, block);

    return block;
}