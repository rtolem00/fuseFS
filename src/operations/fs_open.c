#include <operations/fs_open.h>

#include <fs_inode.h>
#include <structures/fs_file_descriptor.h>
#include <structures/fs_dir.h>

#include <errno.h>

int fs_open(const char *path, struct fuse_file_info *fi)
{
  inode_t* inode;

  if (get_inode_by_path(path, &inode) != 0)
  {
    // No such file or directory
    return -ENOENT;
  }

  // Check permissions
  if ((fi->flags & O_ACCMODE) == O_RDONLY)
  {
    if (!(inode->mode & S_IRUSR))
    {
      return -EACCES; // Permission denied
    }
  }
  else if ((fi->flags & O_ACCMODE) == O_WRONLY)
  {
    if (!(inode->mode & S_IWUSR))
    {
      return -EACCES; // Permission denied
    }
  }
  else if ((fi->flags & O_ACCMODE) == O_RDWR)
  {
    if (!(inode->mode & S_IRUSR) || !(inode->mode & S_IWUSR))
    {
      return -EACCES; // Permission denied
    }
  }

  // Create a new file descriptor
  file_descriptor_t* fd = fd_create(inode, 0, fi->flags & O_ACCMODE);
  if (fd == NULL)
  {
    return -EMFILE; // Too many open files
  }

  // Store the file descriptor in fuse_file_info
  fi->fh = (uint64_t) fd;

  return 0;
}
