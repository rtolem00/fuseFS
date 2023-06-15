#include <structures/fs_dir_entry.h>


dir_t* create_dir(inode_t* inode)
{
  dir_t* dir = (dir_t*) malloc(sizeof(dir_t));
  if(!dir)
  {
    // Handle allocation error
    return NULL;
  }
  dir->inode = inode;
  dir->entries = NULL;
  dir->num_entries = 0;
  return dir;
}

void delete_dir(dir_t* dir)
{
  if(dir != NULL)
  {
    if(dir->entries != NULL)
    {
      if(dir->num_entries == 0)
      {
        free(dir->entries);
      }
    }
    delete_inode(dir->inode);
    free(dir);
  }
}

int add_dir_entry(dir_t* dir, const char* name, uint64_t ino)
{
  dir->entries = (dir_entry_t*) realloc(dir->entries, sizeof(dir_entry_t) * (dir->num_entries + 1));
  if(!dir->entries)
  {
    // Handle allocation error
    return -1;
  }
  strncpy(dir->entries[dir->num_entries].filename, name, 256);
  dir->entries[dir->num_entries].ino = ino;
  dir->num_entries++;
  return 0; // success
}

int remove_dir_entry(dir_t* dir, const char* name)
{
  for(uint64_t i = 0; i < dir->num_entries; i++)
  {
    if(strcmp(dir->entries[i].filename, name) == 0)
    {
      // Shift entries after i one position to the left
      memmove(&dir->entries[i], &dir->entries[i+1], sizeof(dir_entry_t) * (dir->num_entries - i - 1));
      dir->num_entries--;
      dir->entries = (dir_entry_t*) realloc(dir->entries, sizeof(dir_entry_t) * dir->num_entries);
      return 0; // success
    }
  }
  return -1; // name not found
}

dir_entry_t* lookup_dir_entry(dir_t* dir, const char* name)
{
  for(uint64_t i = 0; i < dir->num_entries; i++)
  {
    if(strcmp(dir->entries[i].filename, name) == 0)
    {
      return &dir->entries[i];
    }
  }
  return NULL; // name not found
}
