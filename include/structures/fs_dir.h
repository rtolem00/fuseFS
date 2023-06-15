#ifndef FS_DIR_H
#define FS_DIR_H

#include <fs_inode.h>
#include <structures/fs_dir_entry.h>
#include <stdint.h>


typedef struct dir
{
  inode_t* inode;   // inode of the directory
  dir_entry_t* entries; // list of directory entries
  uint64_t entry_count; // number of entries in the directory
} dir_t;


int create_directory(inode_t* parent_inode, const char* name);
int delete_directory(inode_t* dir_inode);
dir_entry_t* list_directory(inode_t* dir_inode, off_t* offset);
int add_to_directory(inode_t* dir_inode, inode_t* new_inode, const char* name);
int remove_from_directory(inode_t* dir_inode, const char* name);

#endif // FS_DIR_H
