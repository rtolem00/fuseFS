#ifndef FS_RELEASE_H
#define FS_RELEASE_H

#include <fuse.h>

int fs_release(const char* path, struct fuse_file_info* fi);

#endif // FS_RELEASE_H
