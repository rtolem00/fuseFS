#include <operations/fs_mkdir.h>
#include <fs_inode.h>
#include <structures/fs_dir.h>
#include <libgen.h>
#include <sys/stat.h> // Include sys/stat.h to use S_IFREG

#include <errno.h>

int fs_mkdir(const char *path, mode_t mode)
{
  inode_t* existing_inode = NULL;
  
  if (get_inode_by_path(path, &existing_inode) != 0)
  {
    // Failed to get the inode
    return -ENOMEM; // Out of memory
  }

  if (existing_inode != NULL)
  {
    // Directory entry already exists
    return -EEXIST; // File exists
  }

  const char* parent_dir_name = get_parent_path(path);
  const char* dir_name = basename(path);

  if (parent_dir_name == NULL)
  {
    // Failed to get the parent directory's name
    return -ENOMEM; // Out of memory
  }

  if (dir_name == NULL)
  {
    // Failed to get the directory's name
    free(parent_dir_name);
    return -ENOMEM; // Out of memory
  }

  inode_t* parent_inode = NULL;
  
  if (get_inode_by_path(parent_dir_name, &parent_inode) != 0)
  {
    // Failed to get the parent directory's inode
    free(parent_dir_name);
    free(dir_name);
    return -ENOENT; // No such file or directory
  }
  if (parent_inode == NULL)
  {
    // Failed to get the parent directory's inode
    free(parent_dir_name);
    free(dir_name);
    return -ENOENT; // No such file or directory
  }
  if (parent_inode->mode & __S_IFREG)
  {
    // Parent inode is not a directory
    free(parent_dir_name);
    free(dir_name);
    return -ENOTDIR; // Not a directory
  }

  inode_t* inode = create_inode(__S_IFDIR | mode, getuid(), getgid());
  dir_entry_t* dir = create_dir_entry(parent_inode, dir_name);

  if (inode == NULL || dir == NULL)
  {
    // Failed to create the inode or directory entry
    free(parent_dir_name);
    free(dir_name);
    delete_inode(inode);
    delete_dir_entry(dir);
    return -ENOMEM; // Out of memory
  }

  if (add_entry_to_directory(parent_inode, dir) != 0)
  {
    // Failed to add the directory entry
    free(parent_dir_name);
    free(dir_name);
    delete_inode(inode);
    delete_dir_entry(dir);
    return -ENOMEM; // Out of memory
  }

  return 0;
}

char* get_parent_path(const char* path)
{
  char* last_slash = strrchr(path, '/');
  if (last_slash == path)
  {
    // The parent directory is the root directory
    return strdup("/");
  }

  size_t parent_path_length = last_slash - path;
  char* parent_path = (char*) malloc(parent_path_length + 1);
  if (parent_path == NULL)
  {
    // Out of memory
    return NULL;
  }

  strncpy(parent_path, path, parent_path_length);
  parent_path[parent_path_length] = '\0'; // Null-terminate the string
  return parent_path;
}
