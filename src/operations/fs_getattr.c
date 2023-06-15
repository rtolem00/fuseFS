#include <sys/stat.h>
#include "../../include/operations/fs_getattr.h"

int fs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
  int res = 0;

  // Clear the file attribute structure to 0
  memset(stbuf, 0, sizeof(struct stat));

  // Implement your logic here to populate 'stbuf' with file attributes.
  // This could involve querying your file system's data structures and/or underlying storage.
  // Let's assume we're dealing with a regular file for now:

  if (strcmp(path, "/") == 0)
  {
    // Root directory
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  }
  else
  {   
    // Regular file
    stbuf->st_mode = S_IFREG | 0644;
    stbuf->st_nlink = 1;
    stbuf->st_size = 1024;  // replace with your file size
  }

  return res;
}
