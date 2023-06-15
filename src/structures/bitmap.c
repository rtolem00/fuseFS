#include <structures/bitmap.h>

// Assume we have a bitmap for inodes and blocks
static unsigned char inode_bitmap[MAX_INODES / 8]; // Assuming a maximum of 1024 inodes
static unsigned char block_bitmap[MAX_BLOCKS / 8]; // Assuming a maximum of 4096 blocks

uint64_t find_free_inode()
{
  for (uint64_t i = 0; i < MAX_INODES; i++)
  {
    uint64_t byte = i / 8;
    uint64_t bit = i % 8;
    if ((inode_bitmap[byte] & (1 << bit)) == 0)
    {
      // Bit is not set, inode is free
      mark_inode_as_used(i);
      return i;
    }
  }
  return (uint64_t) -1; // No free inodes, handle this in your code
}

void mark_inode_as_free(uint64_t ino)
{
  uint64_t byte = ino / 8;
  uint64_t bit = ino % 8;
  inode_bitmap[byte] &= ~(1 << bit); // Clear the bit
}

void mark_inode_as_used(uint64_t ino)
{
  uint64_t byte = ino / 8;
  uint64_t bit = ino % 8;
  inode_bitmap[byte] |= (1 << bit); // Set the bit
}

uint64_t find_free_block()
{
  for (uint64_t i = 0; i < MAX_BLOCKS; i++)
  {
    uint64_t byte = i / 8;
    uint64_t bit = i % 8;
    if ((block_bitmap[byte] & (1 << bit)) == 0)
    {
      // Bit is not set, block is free
      mark_block_as_used(i);
      return i;
    }
  }
  return (uint64_t) -1; // No free blocks, handle this in your code
}

void mark_block_as_used(uint64_t block)
{
  uint64_t byte = block / 8;
  uint64_t bit = block % 8;
  block_bitmap[byte] |= (1 << bit); // Set the bit
}

void mark_block_as_free(uint64_t block)
{
  uint64_t byte = block / 8;
  uint64_t bit = block % 8;
  block_bitmap[byte] &= ~(1 << bit); // Clear the bit
}
