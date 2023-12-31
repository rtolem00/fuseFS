#ifndef FS_INODE_H
#define FS_INODE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <structures/fs_data_block.h>


#define MAX_INODE_SIZE 4096


typedef struct inode
{
  uint64_t ino;              // i-node number
  mode_t mode;               // file type and permissions
  nlink_t nlink;             // number of links
  uid_t uid;                 // user ID of owner
  gid_t gid;                 // group ID of owner
  off_t size;                // total size in bytes
  time_t atime;              // time of last access
  time_t mtime;              // time of last modification
  time_t ctime;              // time of last status change
  uint64_t block_count;      // number of disk blocks allocated
  uint64_t* blocks;          // array of block numbers
} inode_t;

inode_t* create_inode(mode_t mode, uid_t uid, gid_t gid);
void delete_inode(inode_t* inode);
int read_inode(inode_t* inode, char* buf, size_t size, off_t offset);
int write_inode(inode_t* inode, const char* buf, size_t size, off_t offset);
int change_inode_mode(inode_t* inode, mode_t mode);
int change_inode_owner(inode_t* inode, uid_t uid, gid_t gid);
void update_inode_access_time(inode_t* inode);
void update_inode_modification_time(inode_t* inode);
int write_block_to_inode(inode_t* inode, uint64_t block_index, data_block_t* block);


#endif // FS_INODE_H
