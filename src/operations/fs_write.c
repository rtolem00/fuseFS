#include <operations/fs_write.h>

#include <structures/fs_file_descriptor.h>
#include <fs_inode.h>

#include <errno.h>

int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  file_descriptor_t* fd = fd_find(fi->fh);

  // Check if file descriptor has write access
  if (!(fd->access_mode == O_WRONLY || fd->access_mode == O_RDWR))
  {
    return -EACCES; // Permission denied
  }

  // Write data to the inode associated with the file descriptor
  int write_bytes = write_inode(fd->inode, buf, size, fd->offset + offset);

  if (write_bytes < 0)
  {
    // Error occurred while writing
    return write_bytes;
  }

  return write_bytes;
}
