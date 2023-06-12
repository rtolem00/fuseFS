#ifndef FS_FILE_DESCRIPTOR_H
#define FS_FILE_DESCRIPTOR_H

#include "fs_inode.h"


typedef struct file_descriptor {
  struct inode* inode;
  size_t offset;
} file_descriptor_t;


file_descriptor_t* open_file(inode_t* inode);
void close_file(file_descriptor_t* fd);	
int read_file(file_descriptor_t* fd, char* buf, size_t size);
int write_file(file_descriptor_t* fd, const char* buf, size_t size);

#endif // FS_FILE_DESCRIPTOR_H
