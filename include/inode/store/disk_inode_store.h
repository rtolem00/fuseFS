// disk_bitmap.h
#ifndef _DISK_INODE_STORE_H
#define _DISK_INODE_STORE_H

#include <inode/inode_store.h>


static int disk_store_inode(inode_t* inode);
static inode_t* disk_retrieve_inode(uint64_t ino);
static int disk_update_inode(inode_t* inode);
static int disk_delete_inode(uint64_t ino);
inode_store_t* create_disk_inode_store(const char* inode_file);
void destroy_disk_inode_store();


#endif // _DISK_INODE_STORE_H