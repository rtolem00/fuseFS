#ifndef FS_DIR_ENTRY_H
#define FS_DIR_ENTRY_H

#include "fs_inode.h"
#include <linux/limits.h>

typedef struct dir_entry {
  uint64_t ino;             // i-node number of this entry
  char name[NAME_MAX+1];    // name of this entry
} dir_entry_t;

dir_entry_t* create_dir_entry(const char* name, inode_t* inode);
void delete_dir_entry(dir_entry_t* dir_entry);
inode_t* lookup_dir_entry(dir_entry_t* dir, const char* name);

#endif // FS_DIR_ENTRY_H
