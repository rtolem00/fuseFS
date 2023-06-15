#include <structures/bitmap/block_bitmap.h>


// Singleton instance of the inode store
static block_bitmap_t* block_bitmap;

void set_block_bitmap(bitmap_t* bitmap)
{
  block_bitmap = bitmap;
}

uint64_t find_free_block()
{
  return block_bitmap->find_first_zero();
}

int mark_block_as_used(uint64_t ino)
{
  return block_bitmap->set_bit(ino);
}

int mark_block_as_free(uint64_t ino)
{
  return block_bitmap->clear_bit(ino);
}

int is_block_used(uint64_t ino)
{
  return block_bitmap->is_bit_set(ino);
}