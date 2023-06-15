#ifndef FS_DATA_BLOCK_H
#define FS_DATA_BLOCK_H

#define BLOCK_SIZE 4096

#include <sys/types.h>
#include <stdint.h>

/*typedef enum
{
  DATA_BLOCK,
  META_BLOCK,
  DIRECTORY_BLOCK
} block_type_t;

typedef struct
{
  block_type_t block_type;
  uint64_t block_number;
  uint32_t checksum;
  uint32_t data_length;
} block_header_t;

typedef struct
{
  block_header_t header;
  uint8_t data[BLOCK_SIZE - sizeof(block_header_t)]; // Array to hold the actual data
} data_block_t;
*/

typedef struct data_block
{
  char data[BLOCK_SIZE];
} data_block_t;

data_block_t* allocate_block();
void free_block(data_block_t* block);
int read_block(data_block_t* block, char* buf, size_t size, off_t offset);
int write_block(data_block_t* block, const char* buf, size_t size, off_t offset);

#endif // FS_DATA_BLOCK_H
