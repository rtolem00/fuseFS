#ifndef FS_GETATTR_H
#define FS_GETATTR_H

#include <fuse.h>

int fs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);

#endif // FS_GETATTR_H
