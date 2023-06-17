#include <operations/fs_read.h>

#include <structures/fs_file_descriptor.h>
#include <fs_inode.h>

#include <errno.h>


int fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  file_descriptor_t* fd = fd_find(fi->fh);

  if (fd == NULL)
  {
    // Bad file descriptor
    return -EBADF;
  }

  // Check if file descriptor has read access
  if (!(fd->access_mode == O_RDONLY || fd->access_mode == O_RDWR))
  {
    // Permission denied
    return -EACCES;
  }

  // Read data from the inode associated with the file descriptor
  int read_bytes = read_inode(fd->inode, buf, size, fd->offset + offset);

  if (read_bytes < 0)
  {
    // Error occurred while reading
    return read_bytes;
  }

  return read_bytes;
}
