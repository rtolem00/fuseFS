#include <structures/fs_file_descriptor.h>


file_descriptor_t* open_file(inode_t* inode)
{
  // Allocate memory for the new file descriptor
  // Assign the inode and initialize the offset
  // Return the new file descriptor
}

void close_file(file_descriptor_t* fd)
{
  // Free the memory associated with the file descriptor
}

int read_file(file_descriptor_t* fd, char* buf, size_t size)
{
  // Call read_inode with the file descriptor's inode, buffer, size, and offset
  // Update the file descriptor's offset
  // Return the number of bytes read
}

int write_file(file_descriptor_t* fd, const char* buf, size_t size)
{
  // Call write_inode with the file descriptor's inode
}
