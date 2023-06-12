#include "fs_dir_entry.h"


dir_entry_t* create_dir_entry(const char* name, inode_t* inode)
{
  // Allocate memory for the new directory entry
  // Copy the name into the directory entry
  // Assign the inode
  // Return the new directory entry
}

void delete_dir_entry(dir_entry_t* entry)
{
  // Free the memory associated with the directory entry
}

int compare_dir_entry(dir_entry_t* entry, const char* name)
{
  // Compare the name in the directory entry with the given name
  // Return the result of the comparison
}