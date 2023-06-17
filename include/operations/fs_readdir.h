#ifndef __FS_READDIR_H__
#define __FS_READDIR_H__

#include <fuse.h>

int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

#endif // __FS_READDIR_H__