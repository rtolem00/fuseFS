// disk_bitmap.h
#ifndef DISK_BITMAP_H
#define DISK_BITMAP_H

#include <structures/bitmap.h>
#include <stdio.h>


typedef struct disk_bitmap
{
  FILE* file;
} disk_bitmap_t;

// Creates a new bitmap_t instance that uses a file for storage
// filename: name of the file to use
// Returns a pointer to the new bitmap_t instance, or NULL on error
bitmap_t* create_disk_bitmap(const char* filename);

// Destroys a bitmap_t instance created by create_disk_bitmap
// bitmap: the bitmap_t instance to destroy
void destroy_disk_bitmap(bitmap_t* bitmap);

#endif // DISK_BITMAP_H
