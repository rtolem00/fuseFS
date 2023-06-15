#include <storage/ondisk_backend.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BLOCK_SIZE 4096

static char file_name_buffer[256];
static char* storage_path;

static void disk_write_block(uint64_t block_number, data_block_t* block)
{
  sprintf(file_name_buffer, "%s/block_%llu.bin", storage_path, block_number);
  int fd = open(file_name_buffer, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    perror("open");
    exit(EXIT_FAILURE);
  }
  ssize_t write_result = write(fd, block->data, BLOCK_SIZE);
  if (write_result != BLOCK_SIZE)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }
  close(fd);
}

static data_block_t* disk_read_block(uint64_t block_number)
{
  sprintf(file_name_buffer, "%s/block_%llu.bin", storage_path, block_number);
  int fd = open(file_name_buffer, O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    return NULL;
  }
  data_block_t* block = malloc(sizeof(data_block_t));
  ssize_t read_result = read(fd, block->data, BLOCK_SIZE);
  if (read_result != BLOCK_SIZE)
  {
    perror("read");
    free(block);
    return NULL;
  }
  close(fd);
  return block;
}

storage_backend_t* create_disk_backend(char* path)
{
  storage_path = path;
  return create_backend(disk_write_block, disk_read_block);
}