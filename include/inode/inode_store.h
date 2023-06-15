#ifndef INODE_STORE_H
#define INODE_STORE_H

#include <fs_inode.h>
#include <structures/bitmap.h>


typedef struct inode_store
{
  int (*store_inode)(inode_t* inode);                       // Store an inode in the backend
  inode_t* (*retrieve_inode)(uint64_t ino);                 // Retrieve an inode from the backend
  int (*update_inode)(inode_t* inode);                      // Update an existing inode in the backend
  int (*delete_inode)(uint64_t ino);                        // Delete an inode from the backend
} inode_store_t;


inode_store_t* create_inode_store(int (*store_inode)(inode_t*), inode_t* (*retrieve_inode)(uint64_t), int (*update_inode)(inode_t*), int (*delete_inode)(uint64_t));
void set_inode_store(inode_store_t* store);
int store_inode_to_store(inode_t* inode);
inode_t* retrieve_inode_from_store(uint64_t ino);
int update_inode_in_store(inode_t* inode);
int delete_inode_from_store(uint64_t ino);

#endif // INODE_STORE_H
