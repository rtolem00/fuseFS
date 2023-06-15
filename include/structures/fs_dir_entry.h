#ifndef FS_DIR_ENTRY_H
#define FS_DIR_ENTRY_H

#include <fs_inode.h>
#include <linux/limits.h>

typedef struct dir_entry
{
  uint64_t ino;             // i-node number of this entry
  char filename[NAME_MAX+1];    // name of this entry
} dir_entry_t;

typedef struct dir
{
  inode_t* inode;
  dir_entry_t* entries;
  uint64_t num_entries;
} dir_t;

// Function Prototypes
dir_t* create_dir(inode_t* inode);
void delete_dir(dir_t* dir);
int add_dir_entry(dir_t* dir, const char* name, uint64_t ino);
int remove_dir_entry(dir_t* dir, const char* name);
dir_entry_t* lookup_dir_entry(dir_t* dir, const char* name);

#endif // FS_DIR_ENTRY_H
