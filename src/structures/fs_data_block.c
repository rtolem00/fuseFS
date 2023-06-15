#include <structures/fs_data_block.h>

#include <stdlib.h>


data_block_t* allocate_block()
{
  data_block_t* new_block = (data_block_t*)malloc(sizeof(data_block_t));
  if (new_block == NULL)
  {
    return NULL;
  }

  memset(new_block->data, 0, sizeof(new_block->data));
  return new_block;
}

void free_block(data_block_t* block)
{
  free(block);
}

int read_block(data_block_t* block, char* buf, size_t size, off_t offset)
{
  if(offset + size > BLOCK_SIZE)
  {
    // Error: trying to read past the end of the block
    return -1;
  }

  memcpy(buf, block->data + offset, size);
  return size;
}

int write_block(data_block_t* block, const char* buf, size_t size, off_t offset)
{
  if(offset + size > BLOCK_SIZE)
  {
    // Error: trying to write past the end of the block
    return -1;
  }

  block = block == NULL ? allocate_block() : block;

  if(block == NULL)
  {
    // Error: could not allocate a new block
    return -1;
  }

  memcpy(block->data + offset, buf, size);
  return size;
}
