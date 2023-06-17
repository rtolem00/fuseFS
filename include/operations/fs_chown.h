#ifndef FS_CHOWN_H
#define FS_CHOWN_H

#include <fuse.h>


int fs_chown(const char* path, uid_t uid, gid_t gid);

#endif // FS_CHOWN_H
