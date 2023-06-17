#ifndef FS_STATFS_H
#define FS_STATFS_H

#include <fuse.h>

int fs_statfs(const char *path, struct statvfs *stbuf);

#endif // FS_STATFS_H
