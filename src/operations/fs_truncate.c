#include <operations/fs_truncate.h>

#include <fs_inode.h>
#include <structures/fs_file_descriptor.h>
#include <inode/inode_store.h>

#include <errno.h>

static int my_fs_truncate(const char *path, off_t size)
{
  inode_t* inode;
  int ret = get_inode_by_path(path, &inode);
  
  if (ret != 0)
  {
    // Failed to get the inode
    return ret;
  }

  file_descriptor_t* fd = NULL;
  int ret = fd_find_inode(inode->ino, &fd);

  if (ret != -1)
  {
    // File is opened somewhere else
    return -EBUSY;
  }

  // Change the size of the inode
  ret = change_inode_size(inode, size);
  
  if (ret != 0)
  {
    // Failed to change the size of the inode
    return ret;
  }

  ret = update_inode_in_store(inode);

  return ret;
}
