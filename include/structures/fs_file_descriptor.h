#ifndef FS_FILE_DESCRIPTOR_H
#define FS_FILE_DESCRIPTOR_H

#include <fs_inode.h>


typedef struct file_descriptor
{
  uint64_t fd;
  struct inode* inode;
  size_t offset;
  int access_mode;
} file_descriptor_t;


// File descriptor store operations
file_descriptor_t* fd_create(inode_t *inode, off_t offset, int access_mode);
void fd_destroy(file_descriptor_t *fd);
file_descriptor_t* fd_find(uint64_t fd);

#endif // FS_FILE_DESCRIPTOR_H
