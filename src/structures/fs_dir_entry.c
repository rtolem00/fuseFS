#include <structures/fs_dir_entry.h>
#include <fs_storage_backend.h>


dir_entry_t* create_dir_entry(uint64_t ino, const char* filename)
{
  dir_entry_t* entry = malloc(sizeof(dir_entry_t));
  if (entry == NULL)
  {
    return NULL;
  }
  entry->ino = ino;
  strcpy(entry->filename, filename);
  entry->filename[sizeof(entry->filename)-1] = '\0';

  return entry;
}

void delete_dir_entry(dir_entry_t* entry)
{
  free(entry);
}

dir_entry_t* read_dir_entry(inode_t* inode, off_t offset)
{
  if (offset >= inode->size)
  {
    return NULL;
  }

  dir_entry_t* entry = malloc(sizeof(dir_entry_t));
  if (entry == NULL)
  {
    return NULL;
  }

  // Calculate the block index and the offset within the block
  off_t block_index = offset / BLOCK_SIZE;
  off_t block_offset = (offset % BLOCK_SIZE) / sizeof(dir_entry_t);

  data_block_t* block = read_block_from_storage(inode->blocks[block_index]);
  if (block == NULL)
  {
    return NULL;
  }

  // Copy the directory entry from the block
  read_block(block, entry, sizeof(dir_entry_t), block_offset * sizeof(dir_entry_t));

  // Clean up and return
  free(block);
  return entry;
}

int write_dir_entry(inode_t* dir_inode, dir_entry_t* dirent, off_t offset)
{
  // Check if the offset is valid
  if (offset < 0 || offset >= dir_inode->size)
  {
    // Invalid offset
    return -1;
  }

  // Calculate the block index and the offset within the block
  off_t block_index = offset / BLOCK_SIZE;
  off_t block_offset = (offset % BLOCK_SIZE) / sizeof(dir_entry_t);

  // Read the block from the inode
  if(block_index >= dir_inode->block_count)
  {
    // Error: indirect blocks not implemented
    return -1;
  }
  data_block_t* block = read_block_from_storage(dir_inode->blocks[block_index]);
  if (!block)
  {
    // Error reading the block
    return -1;
  }

  // Write the directory entry to the block
  write_block(block, dirent, sizeof(dir_entry_t), block_offset * sizeof(dir_entry_t));

  // Write the block back to the inode
  if (write_block_to_inode(dir_inode, block_index, block) < 0)
  {
    free(block);
    return -1; // Error writing the block
  }

  // Clean up and return
  free(block);
  return 0;
}
