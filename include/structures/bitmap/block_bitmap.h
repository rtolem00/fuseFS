#ifndef BLOCK_BITMAP_H
#define BLOCK_BITMAP_H


#include <structures/bitmap.h>


typedef bitmap_t block_bitmap_t;

void set_block_bitmap(bitmap_t* bitmap);
uint64_t find_free_block();
int mark_block_as_used(uint64_t ino);
int mark_block_as_free(uint64_t ino);
int is_block_used(uint64_t ino);

#endif // BLOCK_BITMAP_H