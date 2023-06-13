#include "../../include/structures/fs_inode.h"


inode_t* create_inode(mode_t mode, uid_t uid, gid_t gid)
{
  inode_t* inode = malloc(sizeof(inode_t));
  inode->ino = generate_unique_ino();  // Function to generate a unique i-node number
  inode->mode = mode;
  inode->nlink = 0;
  inode->uid = uid;
  inode->gid = gid;
  inode->size = 0;
  inode->atime = current_time();       // Function to get current time
  inode->mtime = current_time();
  inode->ctime = current_time();
  inode->block_count = 0;
  inode->blocks = NULL;

  return inode;
}

void delete_inode(inode_t* inode)
{
  if (inode->blocks)
  {
    for (int i = 0; i < inode->block_count; i++)
    {
      free_block(inode->blocks[i]);  // Function to free data blocks
    }
    free(inode->blocks);
  }
  free(inode);
}

int read_inode(inode_t* inode, char* buf, size_t size, off_t offset)
{
    // Calculate the start block and the offset within the block
    // Read data from the block(s) into the buffer
    // Update the atime field
    // Return the number of bytes read
}

int write_inode(inode_t* inode, const char* buf, size_t size, off_t offset)
{
    // Calculate the start block and the offset within the block
    // Write data from the buffer into the block(s)
    // Update the mtime and ctime fields
    // Return the number of bytes written
}

int change_inode_mode(inode_t* inode, mode_t mode)
{
    // Change the mode of the inode
    // Update the ctime field
    // Return success/failure
}

int change_inode_owner(inode_t* inode, uid_t uid, gid_t gid)
{
    // Change the owner and group of the inode
    // Update the ctime field
    // Return success/failure
}
