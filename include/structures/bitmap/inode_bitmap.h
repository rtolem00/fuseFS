#ifndef INODE_BITMAP_H
#define INODE_BITMAP_H


#include <structures/bitmap.h>


typedef bitmap_t inode_bitmap_t;

void set_inode_bitmap(bitmap_t* bitmap);
uint64_t find_free_inode();
int mark_inode_as_used(uint64_t ino);
int mark_inode_as_free(uint64_t ino);
int is_inode_used(uint64_t ino);

#endif // INODE_BITMAP_H