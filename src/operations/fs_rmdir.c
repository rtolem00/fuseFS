#include <operations/fs_rmdir.h>

#include <fs_inode.h>
#include <structures/fs_dir.h>

#include <errno.h>

int fs_rmdir(const char *path)
{
  inode_t* inode = NULL;
  
  int ret = get_inode_by_path(path, &inode);

  if (ret != 0)
  {
    // Failed to get the inode
    return ret; // Out of memory
  }

  // Check if inode represents a directory
  if ((inode->mode & __S_IFDIR) != __S_IFDIR)
  {
    return -ENOTDIR; // Not a directory
  }

  dir_t* dir = get_dir(inode);

  int ret = delete_directory(dir);

  return ret;
}
