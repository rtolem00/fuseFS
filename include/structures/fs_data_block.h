#ifndef FS_DATA_BLOCK_H
#define FS_DATA_BLOCK_H

#define BLOCK_SIZE 4096

#include <sys/types.h>

typedef struct data_block {
  char data[BLOCK_SIZE];    // array to store actual data
} data_block_t;

data_block_t* allocate_block();
void free_block(data_block_t* block);
int read_block(data_block_t* block, char* buf, size_t size, off_t offset);
int write_block(data_block_t* block, const char* buf, size_t size, off_t offset);

#endif // FS_DATA_BLOCK_H
