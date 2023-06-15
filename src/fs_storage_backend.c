#include <fs_storage_backend.h>
#include <fs_cache.h>


// Singleton instance of the storage backend
static storage_backend_t* backend;

storage_backend_t* create_backend(void (*write_block)(uint64_t, data_block_t*), data_block_t* (*read_block)(uint64_t), int (*delete_block)(uint64_t))
{
  storage_backend_t* new_backend = malloc(sizeof(storage_backend_t));
  new_backend->write_block = write_block;
  new_backend->read_block = read_block;
  new_backend->delete_block = delete_block;
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

int delete_block_from_storage(uint64_t block_number)
{
  return backend->delete_block(block_number);
}
