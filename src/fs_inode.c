#include <fs_inode.h>
#include <structures/fs_data_block.h>

// Function to update the inode access time
void update_inode_access_time(inode_t* inode)
{
  inode->atime = time(NULL);
}

// Function to update the inode modification time
void update_inode_modification_time(inode_t* inode)
{
  inode->mtime = time(NULL);
}

inode_t* create_inode(mode_t mode, uid_t uid, gid_t gid)
{
  uint64_t ino = find_free_inode();
  if (ino == (uint64_t) -1)
  {
    // handle error, no free inodes
    return NULL;
  }

  inode_t* inode = (inode_t*)malloc(sizeof(inode_t));
  if (inode == NULL)
  {
    // handle error, unable to allocate memory
    return NULL;
  }

  // Initialize inode
  inode->ino = ino;
  inode->mode = mode;
  inode->nlink = 0;
  inode->uid = uid;
  inode->gid = gid;
  inode->size = 0;
  inode->atime = time(NULL);
  inode->mtime = time(NULL);
  inode->ctime = time(NULL);
  inode->block_count = 0;
  inode->blocks = NULL;  // inode initially points to no blocks

  return inode;
}

void delete_inode(inode_t* inode)
{
  if (inode->blocks)
  {
    for (int i = 0; i < inode->block_count; i++)
    {
      mark_block_as_free(inode->blocks[i]);  // Function to mark the data block as free
    }
    free(inode->blocks);
  }
  // Function to delete the i-node from the i-node store
  delete_inode_from_store(inode->ino);
  // Function to mark the i-node bitmap as free
  mark_inode_as_free(inode->ino);
  // Free the i-node itself
  free(inode);
}

int read_inode(inode_t* inode, char* buf, size_t size, off_t offset)
{
  // Error checking
  if (offset > inode->size)
  {
    // If offset is beyond the file size, there's nothing to read
    return -1;
  }

  // Reading should not go beyond the file's actual size
  if (size > inode->size - offset)
  {
    size = inode->size - offset;
  }

  // Calculate the starting block index and the offset within that block
  uint64_t block_index = offset / BLOCK_SIZE;
  uint64_t block_offset = offset % BLOCK_SIZE;

  size_t bytes_read = 0;
  while(bytes_read < size)
  {
    // Check if we need to read from a new block
    if(block_offset == BLOCK_SIZE)
    {
      block_index++;
      block_offset = 0;
    }

    // Get the block number from the inode's blocks
    uint64_t block_no = inode->blocks[block_index];

    // Calculate how much to read from this block
    size_t to_read = size - bytes_read;
    if(to_read > BLOCK_SIZE - block_offset)
    {
      to_read = BLOCK_SIZE - block_offset;
    }

    // Read from the block using the backend
    data_block_t* block = read_block_from_storage(block_no);
    if(block == NULL)
    {
      // Error reading from the block
      return -1;
    }

    // Copy the data to the buffer
    int res = read_block(block, buf + bytes_read, to_read, block_offset);

    // Update the amounts
    bytes_read += to_read;
    block_offset += to_read;
  }

  // Update the access time
  update_inode_access_time(inode);

  // Write the inode back to the store
  write_inode_to_store(inode);

  // Return the number of bytes read
  return bytes_read;
}

int write_inode(inode_t* inode, const char* buf, size_t size, off_t offset)
{
  // Calculate the starting block index and the offset within that block
  uint64_t block_index = offset / BLOCK_SIZE;
  uint64_t block_offset = offset % BLOCK_SIZE;

  size_t bytes_written = 0;
  while(bytes_written < size)
  {
    // Check if we need to write into a new block
    if(block_offset == BLOCK_SIZE)
    {
      block_index++;
      block_offset = 0;
    }

    // Check if we need to allocate a new block
    if(block_index >= inode->block_count)
    {
      uint64_t new_block = find_free_block();
      if(new_block == (uint64_t)-1)
      {
        // No free blocks available, handle error
        return -1;
      }

      // Expand the blocks array
      inode->blocks = (uint64_t*)realloc(inode->blocks, (inode->block_count + 1) * sizeof(uint64_t));
      if(inode->blocks == NULL)
      {
        // Failed to allocate memory, handle error
        return -1;
      }

      // Update the blocks array and the block count
      inode->blocks[inode->block_count] = new_block;
      inode->block_count++;

      // Mark the new block as used
      mark_block_as_used(new_block);
    }

    // Get the block number from the inode's blocks
    uint64_t block_no = inode->blocks[block_index];

    // Calculate how much to write into this block
    size_t to_write = size - bytes_written;
    if(to_write > BLOCK_SIZE - block_offset)
    {
      to_write = BLOCK_SIZE - block_offset;
    }

    // Read the current block data if we need to preserve some of it
    data_block_t* block = NULL;
    if(block_offset > 0 || to_write < BLOCK_SIZE)
    {
      block = read_block_from_storage(block_no);
      if(block == NULL)
      {
        // Error reading from the block
        return -1;
      }
    }

    // Copy the new data to the block
    int res = write_block(block, buf + bytes_written, to_write, block_offset);
    if(res == -1)
    {
      // Error writing to the block
      return -1;
    }
    
    // Write the block data to the storage
    write_block_to_storage(block_no, block);

    // Update the amounts
    bytes_written += to_write;
    block_offset += to_write;

    // Free the block data
    free_block(block);
  }

  // Update the inode size if necessary
  if(offset + bytes_written > inode->size)
  {
    inode->size = offset + bytes_written;
  }

  // Update the access and modification times of the inode
  update_inode_access_time(inode);
  update_inode_modification_time(inode);

  // Write the inode back to the store
  write_inode_to_store(inode);

  // Return the number of bytes written
  return bytes_written;
}

int change_inode_mode(inode_t* inode, mode_t mode)
{
  if (inode == NULL)
  {
    // If the inode is NULL, there's nothing to do
    return -1;
  }

  // Change the mode
  inode->mode = mode;
  // Update the ctime field
  update_inode_modification_time(inode);

  // Return success
  return 0;
}

int change_inode_owner(inode_t* inode, uid_t uid, gid_t gid)
{
  if (inode == NULL)
  {
    // If the inode is NULL, there's nothing to do
    return -1;
  }

  // Change the owner and group
  inode->uid = uid;
  inode->gid = gid;
  // Update the ctime field
  update_inode_modification_time(inode);

  // Return success
  return 0;
}
