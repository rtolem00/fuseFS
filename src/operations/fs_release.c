#include <operations/fs_release.h>
#include <structures/fs_file_descriptor.h>

#include <errno.h>

int fs_release(const char *path, struct fuse_file_info *fi)
{
  file_descriptor_t* fd = fd_find(fi->fh);

  // Check if the file descriptor exists
  if (fd == NULL)
  {
    return -EBADF; // Bad file descriptor
  }

  // Remove the file descriptor from the store
  fd_destroy(fd);

  return 0;
}
