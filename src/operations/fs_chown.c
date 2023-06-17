#include <operations/fs_chown.h>

#include <fs_inode.h>
#include <structures/fs_dir.h>
#include <inode/inode_store.h>

int fs_chown(const char *path, uid_t uid, gid_t gid)
{
  inode_t* inode;
  int ret = get_inode_by_path(path, &inode);
  if (ret != 0)
  {
    // Failed to get the inode
    return ret;
  }

  // Change the inode owner
  ret = change_inode_owner(inode, uid, gid);
  if (ret != 0)
  {
    // Failed to change the inode owner
    return ret;
  }

  update_inode_in_store(inode);

  return 0;
}
