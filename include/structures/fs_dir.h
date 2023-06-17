#ifndef FS_DIR_H
#define FS_DIR_H

#include <fs_inode.h>
#include <structures/fs_dir_entry.h>
#include <stdint.h>

static dir_t root_dir;

typedef struct dir
{
  inode_t* inode;   // inode of the directory
  dir_entry_t* entries; // list of directory entries
  uint64_t entry_count; // number of entries in the directory
} dir_t;


dir_t* create_directory(const char* name, mode_t mode);
int delete_directory(dir_t* dir);
dir_t* get_dir(inode_t* inode);
int add_entry_to_directory(dir_t* dir, dir_entry_t* entry);
dir_t* get_dir(uint64_t ino);
int list_directory(dir_t* dir, char*** names);
dir_entry_t* list_directory(inode_t* dir_inode, off_t* offset);
int add_to_directory(dir_t* dir, const char* name, uint64_t ino);
int remove_from_directory(dir_t* dir, const char* name);
dir_entry_t* get_dir_entry(dir_t* dir, const char* name);
int set_root_dir(dir_t* dir);
dir_t* get_root_dir();
int get_inode_by_path(const char* path, inode_t** inode);
int create_root_dir(const char* path);

#endif // FS_DIR_H
