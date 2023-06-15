// disk_bitmap.h
#ifndef DISK_BITMAP_H
#define DISK_BITMAP_H

#include <inode/inode_store.h>


static int disk_store_inode(inode_t* inode);
static inode_t* disk_retrieve_inode(uint64_t ino);
static int disk_update_inode(inode_t* inode);
static int disk_delete_inode(uint64_t ino);
inode_store_t* create_disk_inode_store(const char* inode_file);
void destroy_disk_inode_store();


#endif // DISK_BITMAP_H