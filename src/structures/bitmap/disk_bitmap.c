// disk_bitmap.c
#include <structures/bitmap/disk_bitmap.h>


static int disk_set_bit(bitmap_t* bitmap, uint64_t bit)
{
  disk_bitmap_t* disk_bitmap = (disk_bitmap_t*)bitmap->private;
  fseek(disk_bitmap->file, bit / 8, SEEK_SET);
  uint8_t byte;
  fread(&byte, 1, 1, disk_bitmap->file);
  byte |= 1 << (bit % 8);
  fseek(disk_bitmap->file, bit / 8, SEEK_SET);
  fwrite(&byte, 1, 1, disk_bitmap->file);
  return 0;
}

static int disk_clear_bit(bitmap_t* bitmap, uint64_t bit)
{
  disk_bitmap_t* disk_bitmap = (disk_bitmap_t*)bitmap->private;
  fseek(disk_bitmap->file, bit / 8, SEEK_SET);
  uint8_t byte;
  fread(&byte, 1, 1, disk_bitmap->file);
  byte &= ~(1 << (bit % 8));
  fseek(disk_bitmap->file, bit / 8, SEEK_SET);
  fwrite(&byte, 1, 1, disk_bitmap->file);
  return 0;
}

static uint64_t disk_find_first_zero(bitmap_t* bitmap)
{
  disk_bitmap_t* disk_bitmap = (disk_bitmap_t*)bitmap->private;
  fseek(disk_bitmap->file, 0, SEEK_SET);
  uint8_t byte;
  uint64_t bit = 0;
  while (fread(&byte, 1, 1, disk_bitmap->file))
  {
    if (byte != 0xFF)
    {
      // if not all bits are set
      for (int i = 0; i < 8; i++)
      {
        if (!(byte & (1 << i)))
        {
          return bit + i;
        }
      }
    }
    bit += 8;
  }
  return bit; // all bits are set, return the next bit
}

static int disk_is_set(bitmap_t* bitmap, uint64_t bit)
{
  disk_bitmap_t* disk_bitmap = (disk_bitmap_t*)bitmap->private;
  fseek(disk_bitmap->file, bit / 8, SEEK_SET);
  uint8_t byte;
  fread(&byte, 1, 1, disk_bitmap->file);
  return (byte & (1 << (bit % 8))) != 0;
}

bitmap_t* create_disk_bitmap(const char* filename)
{
  FILE* file = fopen(filename, "r+b");
  if (!file)
  {
    return NULL;
  }

  disk_bitmap_t* disk_bitmap = malloc(sizeof(disk_bitmap_t));
  if (!disk_bitmap)
  {
    fclose(file);
    return NULL;
  }
  disk_bitmap->file = file;

  bitmap_t* bitmap = create_bitmap(disk_set_bit, disk_clear_bit, disk_find_first_zero, disk_is_set);
  if (bitmap == NULL)
  {
    free(disk_bitmap);
    fclose(file);
    return NULL;
  }
  bitmap->private = disk_bitmap;

  return bitmap;
}

void destroy_disk_bitmap(bitmap_t* bitmap)
{
  disk_bitmap_t* disk_bitmap = (disk_bitmap_t*)bitmap->private;
  fclose(disk_bitmap->file);
  free(disk_bitmap);
  free(bitmap);
}
