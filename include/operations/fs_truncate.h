#ifndef FS_TRUNCATE_H
#define FS_TRUNCATE_H

#include <fuse.h>


int fs_truncate(const char* path, off_t size);

#endif // FS_TRUNCATE_H
