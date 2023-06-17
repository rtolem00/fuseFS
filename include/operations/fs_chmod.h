#ifndef FS_CHMOD_H
#define FS_CHMOD_H

#include <fuse.h>

int fs_chmod(const char *path, mode_t mode, struct fuse_file_info *fi);

#endif // FS_CHMOD_H
