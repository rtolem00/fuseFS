#include <structures/fs_dir.h>
#include <inode/inode_store.h>
#include <fs_storage_backend.h>
#include <sys/stat.h>
#include <errno.h>


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
  if ((inode->mode & __S_IFMT) != __S_IFDIR)
  {
    // Not a directory
    free(inode);
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
  dir->entry_count = 0;
  dir->entries = NULL;

  // Read the directory entries
  for (uint64_t i = 0; i < inode->block_count; i++)
  {
    data_block_t* block = read_block_from_storage(inode->blocks[i]);
    if (block == NULL)
    {
      // Error reading block
      free_directory(dir);
      return NULL;
    }

    // Add the entries from the block to the directory
    for (off_t offset = 0; offset < BLOCK_SIZE; offset += sizeof(dir_entry_t))
    {
      dir_entry_t* entry = (dir_entry_t*)&block->data[offset];
      if (entry->ino != 0)
      {
        // Add the entry to the directory
        if (add_entry_to_directory(dir, entry) != 0)
        {
          // Error adding entry to directory
          free(block);
          free_directory(dir);
          return NULL;
        }
      }
    }

    // Free the block
    free(block);
  }

  return dir;
}

int add_entry_to_directory(dir_t* dir, dir_entry_t* entry)
{
  // Check if the entry already exists
  for (uint64_t i = 0; i < dir->entry_count; i++)
  {
    if (strcmp(dir->entries[i].filename, entry->filename) == 0)
    {
      // Entry already exists
      return -EEXIST;
    }
  }

  // Allocate memory for the new entry
  dir_entry_t* new_entry = malloc(sizeof(dir_entry_t));
  if (!new_entry)
  {
    // Out of memory
    return -ENOMEM;
  }

  // Copy the entry
  memcpy(new_entry, entry, sizeof(dir_entry_t));

  // Add the entry to the directory
  dir->entries = realloc(dir->entries, (dir->entry_count + 1) * sizeof(dir_entry_t));
  if (!dir->entries)
  {
    // Out of memory
    free(new_entry);
    return -ENOMEM;
  }
  dir->entries[dir->entry_count] = *new_entry;
  dir->entry_count++;

  return 0;
}

dir_t* get_dir(uint64_t ino)
{
  // Retrieve the inode
  inode_t* inode = retrieve_inode_from_store(ino);
  
  return get_dir(inode);
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

int remove_from_directory(dir_t* dir, const char* name)
{
  // Iterate over all directory entries
  for (uint64_t i = 0; i < dir->inode->block_count; i++)
  {
    // Read the data block
    data_block_t* block = read_block_from_storage(dir->inode->blocks[i]);
    if (!block)
    {
      return -EIO; // Error reading block
    }

    // Iterate over the entries in the block
    for (off_t offset = 0; offset < BLOCK_SIZE; offset += sizeof(dir_entry_t))
    {
      dir_entry_t* entry = (dir_entry_t*)&block->data[offset];

      // Check if the entry matches the name we're looking for
      if (entry->ino != 0 && strncmp(entry->filename, name, sizeof(entry->filename)) == 0)
      {
        inode_t* inode = retrieve_inode_from_store(entry->ino);
        if (!inode)
        {
          // Error retrieving inode
          return -EIO;
        }
        delete_inode(inode);
        delete_dir_entry(entry);
        free(block);

        return 0; // Success
      }
    }
    free(block);
  }

  // Entry not found
  return -ENOENT;
}

dir_entry_t* get_dir_entry(dir_t* dir, const char* name)
{
  // Check for null arguments
  if (dir == NULL || name == NULL)
  {
    return NULL;
  }

  // Iterate through all the entries in the directory
  for (size_t i = 0; i < dir->entry_count; i++)
  {
    dir_entry_t* entry = read_dir_entry(dir->inode, i);
    if (entry != NULL && strcmp(entry->filename, name) == 0)
    {
      return entry;
    }
    free(entry);
  }

  // If no matching entry was found, return NULL
  return NULL;
}

int get_inode_by_path(const char* path, inode_t** inode)
{
  char* token;
  char* rest = strdup(path);
  dir_t* current_dir = root_dir;
  // Initially set the inode to NULL
  *inode = NULL;

  while ((token = strtok_r(rest, "/", &rest)))
  {
    dir_entry_t* dir_entry = get_dir_entry(current_dir, token);
    if (!dir_entry)
    {
      free(rest);
      // No such file or directory
      return -ENOENT;
    }

    *inode = retrieve_inode_from_store(dir_entry->ino);
        
    if (__S_ISDIR((*inode)->mode))
    {
      current_dir = get_dir(inode);
    }
  }

  free(rest);

  // If the loop finished without finding the inode, return an error
  if (*inode == NULL)
  {
    // No such file or directory
    return -ENOENT;
  }

  return inode;
}

int set_root_dir(dir_t* dir)
{
  if (!dir)
  {
    return -EINVAL;
  }
  root_dir = dir;
  return 0;
}

dir_t* get_root_dir()
{
  return root_dir;
}

int create_root_dir(const char* path)
{
  // Create a root directory inode
  inode_t* root_inode = create_inode(__S_IFDIR | 0755, getuid(), getgid());
  if (root_inode == NULL)
  {
    // Not enough memory
    return -ENOMEM;
  }

  // Allocate memory for the directory
  dir_t* dir = malloc(sizeof(dir_t));
  if (dir == NULL)
  {
    delete_inode(root_inode);
    // Not enough memory
    return -ENOMEM;
  }

  // Initialize the directory
  dir->inode = root_inode;
  dir->entries = NULL;
  dir->entry_count = 0;

  // Create a "." directory entry in the root directory
  if (add_to_directory(dir, ".", root_inode->ino) != 0)
  {
    delete_directory(dir);
    delete_inode(root_inode);
    return -EIO; // I/O error
  }

  // Create a ".." directory entry in the root directory
  // The inode number for ".." is the same as "." in the root directory
  if (add_to_directory(dir, "..", root_inode->ino) != 0)
  {
    delete_directory(dir);
    delete_inode(root_inode);
    return -EIO; // I/O error
  }

  // Store the inode
  if (store_inode_to_store(root_inode) < 0)
  {
    delete_directory(dir);
    delete_inode(root_inode);
    return -EIO; // I/O error
  }

  // Set the root directory
  if (set_root_dir(dir) != 0)
  {
    delete_directory(dir);
    delete_inode(root_inode);
    return -EIO; // I/O error
  }

  return 0; // Success
}
