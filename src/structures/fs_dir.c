#include <structures/fs_dir.h>
#include <inode/inode_store.h>
#include <fs_storage_backend.h>
#include <sys/stat.h>
#include <errno.h>


int remove_from_directory(inode_t* dir_inode, const char* name) {
    // Should find the dirent with the given name, remove it, and delete the inode if no more references to it exist
    return -1;
}

dir_t* create_directory(const char* name, mode_t mode)
{
  // Allocate memory for the directory
  dir_t* dir = malloc(sizeof(dir_t));
  if (!dir)
  {
    // Out of memory
    return NULL;
  }

  // Allocate a new inode for the directory
  inode_t* inode = create_inode(mode | 0040000/*S_IFDIR*/, getuid(), getgid());
  if (!inode)
  {
    // Error creating the inode
    free(dir);
    return NULL;
  }

  // Initialize the directory
  dir->inode = inode;
  dir->entries = NULL;
  dir->entry_count = 0;

  // Add the '.' and '..' entries
  if (add_to_directory(dir, inode, ".") < 0 || add_to_directory(dir, inode, "..") < 0)
  {
    // Error adding the entries
    delete_directory(dir);
    return NULL;
  }

  // Store the inode
  if (store_inode_to_store(inode) < 0)
  {
    // Error storing the inode
    delete_directory(dir);
    return NULL;
  }

  return dir;
}

int delete_directory(dir_t* dir)
{
  // Check if the directory is empty (excluding '.' and '..')
  if (dir->entry_count > 2)
  {
    return -ENOTEMPTY; // Directory not empty
  }

  // Remove the directory's entry from its parent directory
  inode_t* parent_inode = retrieve_inode_from_store(dir->inode->ino);
  if (!parent_inode)
  {
    // Parent directory not found
    return -ENOENT;
  }

  // Find the directory's entry in the parent directory
  dir_t* parent_dir = get_dir(parent_inode); // Get parent directory
  for (uint64_t i = 0; i < parent_dir->entry_count; i++)
  {
    if (parent_dir->entries[i].ino == dir->inode->ino)
    {
      // Remove the directory's entry
      memmove(&parent_dir->entries[i], &parent_dir->entries[i + 1],
              (parent_dir->entry_count - i - 1) * sizeof(dir_entry_t));
      parent_dir->entry_count--;
      break;
    }
  }

  // Delete the directory's inode
  delete_inode(dir->inode);
  dir->inode = NULL;

  // Free the memory used by the directory
  free(dir->entries);
  free(dir);

  return 0;
}

dir_t* get_dir(inode_t* inode)
{
  // Check if the inode is a directory
  if (!(inode->mode & 0040000/*S_IFDIR*/))
  {
    // Not a directory
    return NULL;
  }

  // Allocate memory for the directory
  dir_t* dir = malloc(sizeof(dir_t));
  if (!dir)
  {
    // Out of memory
    return NULL;
  }

  // Initialize the directory
  dir->inode = inode;
  dir->entry_count = inode->size / sizeof(dir_entry_t);
  dir->entries = malloc(inode->size);
  if (!dir->entries)
  {
    // Out of memory
    free(dir);
    return NULL;
  }

  // Read the directory entries from the inode's data blocks
  for (uint64_t i = 0; i < inode->block_count; i++)
  {
    if (read_block_from_storage(inode->blocks[i], &dir->entries[i * (BLOCK_SIZE / sizeof(dirent_t))]) < 0)
    {
      // Error reading the block
      free(dir->entries);
      free(dir);
      return NULL;
    }
  }

  return dir;
}

int list_directory(dir_t* dir, char*** names)
{
  // Allocate memory for the names
  *names = malloc(dir->entry_count * sizeof(char*));
  if (!*names)
  {
    // Out of memory
    return -ENOMEM;
  }

  // Get the names of the directory entries
  for (uint64_t i = 0; i < dir->entry_count; i++)
  {
    // Allocate memory for the name
    (*names)[i] = malloc(strlen(dir->entries[i].filename) + 1);
    if (!(*names)[i])
    {
      // Free the already allocated names
      for (uint64_t j = 0; j < i; j++)
      {
        free((*names)[j]);
      }
      free(*names);
      // Out of memory
      return -ENOMEM;
    }

    // Copy the name
    strcpy((*names)[i], dir->entries[i].filename);
  }

  return dir->entry_count;
}

dir_entry_t* list_directory(inode_t* dir_inode, off_t* offset)
{
  // Check if the inode is a directory
  if (!(dir_inode->mode & 0040000/*S_IFDIR*/))
  {
    // Not a directory
    return NULL;
  }

  // Check if the offset is valid
  if (*offset < 0 || *offset >= dir_inode->size)
  {
    // Invalid offset
    return NULL;
  }

  // Get the directory from the inode
  dir_t* dir = get_dir(dir_inode);
  if (!dir)
  {
    // Error getting the directory
    return NULL;
  }

  // Get the directory entry
  dir_entry_t* entry = &dir->entries[*offset / sizeof(dir_entry_t)];

  // Update the offset to the next entry
  *offset += sizeof(dir_entry_t);

  return entry;
}

int add_to_directory(dir_t* dir, const char* name, uint64_t ino)
{
  // Check if there's room for a new entry
  if (dir->inode->size + sizeof(dir_entry_t) > dir->inode->block_count * BLOCK_SIZE)
  {
    // Allocate a new block
    uint64_t block_number = find_free_block();
    if (block_number == UINT64_MAX)
    {
      return -ENOSPC; // No space left on device
    }
    mark_block_as_used(block_number);

    // Update the inode
    dir->inode->blocks = realloc(dir->inode->blocks, (dir->inode->block_count + 1) * sizeof(uint64_t));
    if (!dir->inode->blocks)
    {
      return -ENOMEM; // Out of memory
    }
    dir->inode->blocks[dir->inode->block_count] = block_number;
    dir->inode->block_count++;
  }

  // Add the new directory entry
  dir_entry_t entry;
  strncpy(entry.filename, name, sizeof(entry.filename) - 1);
  entry.filename[sizeof(entry.filename) - 1] = '\0'; // Ensure null-termination
  entry.ino = ino;

  // Write the entry to the directory's last block
  data_block_t* block = read_block_from_storage(dir->inode->blocks[dir->inode->block_count - 1]);
  if (!block)
  {
    // I/O error
    return -EIO;
  }
  memcpy(&block->data[dir->inode->size % BLOCK_SIZE], &entry, sizeof(entry));

  // Update the inode and write the block back to storage
  dir->inode->size += sizeof(entry);
  write_block_to_storage(dir->inode->blocks[dir->inode->block_count - 1], &block);

  return 0; // Success
}
