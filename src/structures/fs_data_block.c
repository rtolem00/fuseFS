#include "../../include/structures/fs_data_block.h"


data_block_t* allocate_block()
{
  // Find a free block in the block bitmap
  // Update the block bitmap to mark the block as used
  // Return the new block
}

void free_block(data_block_t* block)
{
  // Update the block bitmap to mark the block as free
}

int read_block(data_block_t* block, char* buf, size_t size, off_t offset)
{
  // Copy data from the block into the buffer
  // Return the number of bytes read
}

int write_block(data_block_t* block, const char* buf, size_t size, off_t offset)
{
  // Copy data from the buffer into the block
  // Return the number of bytes written
}
