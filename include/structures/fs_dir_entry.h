#ifndef FS_DIR_ENTRY_H
#define FS_DIR_ENTRY_H

#include <fs_inode.h>
#include <linux/limits.h>

typedef struct dir_entry
{
  uint64_t ino;             // i-node number of this entry
  char filename[NAME_MAX+1];    // name of this entry
} dir_entry_t;


// Function Prototypes
dir_entry_t* create_dir_entry(uint64_t ino, const char* filename);
void delete_dir_entry(dir_entry_t* entry);
dir_entry_t* read_dir_entry(inode_t* inode, off_t offset);
int write_dir_entry(inode_t* inode, dir_entry_t* entry, off_t offset);

#endif // FS_DIR_ENTRY_H
