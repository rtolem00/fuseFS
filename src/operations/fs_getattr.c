#include <sys/stat.h>
#include <operations/fs_getattr.h>
#include <errno.h>
#include <stddef.h> // Add this line
#include <fs_inode.h>

int fs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
  memset(stbuf, 0, sizeof(struct stat));

  // root directory
  if (strcmp(path, "/") == 0)
  {
    stbuf->st_mode = __S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  }
  else
  {
    // find the inode
    inode_t* inode = get_inode_by_path(path);

    if (inode == NULL)
    {
      // No such file or directory
      return -ENOENT;
    }
        
    stbuf->st_mode = inode->mode;
    stbuf->st_nlink = inode->nlink;
    stbuf->st_uid = inode->uid;
    stbuf->st_gid = inode->gid;
    stbuf->st_size = inode->size;
    stbuf->st_atime = inode->atime;
    stbuf->st_mtime = inode->mtime;
    stbuf->st_ctime = inode->ctime;
    stbuf->st_blocks = inode->block_count;
  }

  return 0;
}
