#include <operations/fs_readdir.h>
#include <structures/fs_dir.h>
#include <inode/inode_store.h>
#include <stddef.h> // Add this line
#include <errno.h>

int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
  // find the inode
  inode_t* inode;

  if (get_inode_by_path(path, &inode) != 0)
  {
    // No such file or directory
    return -ENOENT;
  }

  // check if inode is a directory
  if (!S_ISDIR(inode->mode))
  {
    return -ENOTDIR; // Not a directory
  }

  dir_t* dir = get_dir(inode);

  struct stat st;
  memset(&st, 0, sizeof(struct stat));
  off_t off = 0;
  dir_entry_t* de;
  while ((de = list_directory(inode, &off)) != NULL)
  {
    inode_t* inode = retrieve_inode_from_storage(de->ino);
    if (inode == NULL)
    {
      return -ENOENT;
    }
    st.st_ino = de->ino;
    st.st_mode = inode->mode;
    st.st_nlink = inode->nlink;
    st.st_uid = inode->uid;
    st.st_gid = inode->gid;
    st.st_size = inode->size;
    st.st_atime = inode->atime;
    st.st_mtime = inode->mtime;
    st.st_ctime = inode->ctime;
    st.st_blocks = inode->block_count;
    filler(buf, de->filename, &st, 0, 0);
    free(inode);
    free(de);
  }

  return 0;
}
