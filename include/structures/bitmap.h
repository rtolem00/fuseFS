#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

#define MAX_INODES 1024
#define MAX_BLOCKS 4096

// Function prototypes
uint64_t find_free_inode();
void mark_inode_as_free(uint64_t ino);
void mark_inode_as_used(uint64_t ino);

uint64_t find_free_block();
void mark_block_as_used(uint64_t block);
void mark_block_as_free(uint64_t block);

#endif // BITMAP_H
