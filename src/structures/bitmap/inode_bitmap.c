#include <structures/bitmap/inode_bitmap.h>


// Singleton instance of the inode store
static inode_bitmap_t* inode_bitmap;

void set_inode_bitmap(bitmap_t* bitmap)
{
  inode_bitmap = bitmap;
}

uint64_t find_free_inode()
{
  return inode_bitmap->find_first_zero();
}

int mark_inode_as_used(uint64_t ino)
{
  return inode_bitmap->set_bit(ino);
}

int mark_inode_as_free(uint64_t ino)
{
  return inode_bitmap->clear_bit(ino);
}

int is_inode_used(uint64_t ino)
{
  return inode_bitmap->is_bit_set(ino);
}
