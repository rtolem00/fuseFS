#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>


typedef struct bitmap
{
  void* private;
  int (*set_bit)(uint64_t bit);
  int (*clear_bit)(uint64_t bit);
  uint64_t (*find_first_zero)();
  int (*is_bit_set)(uint64_t bit);
} bitmap_t;

// Function prototypes
bitmap_t* create_bitmap(int (*set_bit)(uint64_t), int (*clear_bit)(uint64_t), uint64_t (*find_first_zero)(), int (*is_bit_set)(uint64_t));
int set_bit(bitmap_t* bitmap, uint64_t bit);
int clear_bit(bitmap_t* bitmap, uint64_t bit);
uint64_t find_first_zero(bitmap_t* bitmap);
int is_bit_set(bitmap_t* bitmap, uint64_t bit);
void mark_block_as_free(bitmap_t* bitmap, uint64_t block);

#endif // BITMAP_H
