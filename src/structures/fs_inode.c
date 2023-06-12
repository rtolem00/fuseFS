#include "../../include/structures/fs_inode.h"


inode_t* create_inode(mode_t mode, uid_t uid, gid_t gid)
{
    // Allocate memory for the new inode
    // Assign the mode, uid, and gid
    // Initialize other fields to their default values
    // Return the new inode
}

void delete_inode(inode_t* inode)
{
    // Free the memory associated with the inode
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
