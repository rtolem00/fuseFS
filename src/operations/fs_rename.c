#include <operations/fs_rename.h>

#include <fs_inode.h>
#include <libgen.h>
#include <structures/fs_dir.h>

#include <errno.h>

int fs_rename(const char *from, const char *to)
{
  char* parent_path = get_parent_path(from);
  if (parent_path == NULL)
  {
    // Failed to get the parent directory's name
    return -ENOMEM; // Out of memory
  }

  inode_t* parent_inode = NULL;
  int ret = get_inode_by_path(from, &parent_inode);
  free(parent_path);
  if (ret != 0)
  {
    // Failed to get the parent directory's inode
    return ret;
  }

  dir_t* parent_dir = get_dir(parent_inode);
  if (parent_dir == NULL)
  {
    // Failed to get the parent directory
    return -ENOMEM; // Out of memory
  }

  inode_t* inode = NULL;
  ret = get_inode_by_path(from, &inode);
  if (ret != 0)
  {
    // Failed to get the inode
    return ret; // Out of memory
  }

  ret = remove_from_directory(parent_dir, basename(from));
  if (ret != 0)
  {
    // Failed to remove the entry from the directory
    return ret;
  }

  dir_entry_t* new_entry = create_dir_entry(inode->ino, basename(to));
  if (new_entry == NULL)
  {
    // Failed to create the new directory entry
    return -ENOMEM; // Out of memory
  }

  ret = add_entry_to_directory(parent_dir, new_entry);
  
  return ret;
}
