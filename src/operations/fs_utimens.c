#include <operations/fs_utimens.h>

#include <fs_inode.h>
#include <inode/inode_store.h>


int fs_utimens(const char* path, const struct timespec ts[2], struct fuse_file_info* fi)
{
  inode_t* inode;
  int ret = get_inode_by_path(path, &inode);
  if (ret != 0)
  {
    // Failed to get the inode
    return ret;
  }

  inode->atime = ts[0].tv_sec;
  inode->mtime = ts[1].tv_sec;

  // write changes to disk
  ret = update_inode_in_store(inode);
  if (ret != 0)
  {
    // Failed to write the inode to storage
    return ret;
  }

  return 0;
}

