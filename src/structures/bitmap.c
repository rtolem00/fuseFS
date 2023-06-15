#include <structures/bitmap.h>
#include <stdlib.h>


bitmap_t* create_bitmap(int (*set_bit)(uint64_t), int (*clear_bit)(uint64_t), uint64_t (*find_first_zero)(), int (*is_bit_set)(uint64_t))
{
  bitmap_t* bitmap = malloc(sizeof(bitmap_t));
  if (bitmap == NULL)
  {
    return NULL;
  }
  bitmap->set_bit = set_bit;
  bitmap->clear_bit = clear_bit;
  bitmap->find_first_zero = find_first_zero;
  bitmap->is_bit_set = is_bit_set;
  return bitmap;
}

int set_bit(bitmap_t* bitmap, uint64_t bit)
{
  return bitmap->set_bit(bit);
}

int clear_bit(bitmap_t* bitmap, uint64_t bit)
{
  return bitmap->clear_bit(bit);
}

uint64_t find_first_zero(bitmap_t* bitmap)
{
  return bitmap->find_first_zero();
}

int is_bit_set(bitmap_t* bitmap, uint64_t bit)
{
  return bitmap->is_bit_set(bit);
}
