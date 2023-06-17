#include <operations/fs_chmod.h>

#include <fs_inode.h>
#include <structures/fs_dir.h>
#include <inode/inode_store.h>

int fs_chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
{
  inode_t* inode;
  int ret = get_inode_by_path(path, &inode);
  if (ret != 0)
  {
    // Failed to get the inode
    return ret;
  }

  // Change the inode mode
  ret = change_inode_mode(inode, mode);
  if (ret != 0)
  {
    // Failed to change the inode mode
    return ret;
  }

  update_inode_in_store(inode);

  return 0;
}
