#include <structures/fs_file_descriptor.h>


// Initialize the File descriptor store
static file_descriptor_t** fd_store = NULL;
static uint64_t fd_count = 0;

file_descriptor_t* fd_create(inode_t *inode, off_t offset, int access_mode)
{
  // Allocate memory for a new file descriptor
  file_descriptor_t* fd = (file_descriptor_t*) malloc(sizeof(file_descriptor_t));
  if (fd == NULL)
  {
    // Out of memory
    return NULL;
  }

  // Fill in the details
  fd->fd = fd_count++;
  fd->inode = inode;
  fd->offset = offset;
  fd->access_mode = access_mode;

  // Add it to the store
  fd_store = (file_descriptor_t**) realloc(fd_store, sizeof(file_descriptor_t*) * fd_count);
  fd_store[fd->fd] = fd;

  return fd;
}

void fd_destroy(file_descriptor_t *fd)
{
  if (fd == NULL)
  {
    return;
  }
    
  // Remove it from the store
  fd_store[fd->fd] = NULL;
    
  // Free the memory
  free(fd);
}

file_descriptor_t* fd_find(uint64_t fd)
{
  if (fd >= fd_count)
  {
    // No such file descriptor
    return NULL;
  }
    
  return fd_store[fd];
}

int fd_find_inode(uint64_t ino, file_descriptor_t** fd)
{
  for (uint64_t i = 0; i < fd_count; i++)
  {
    if (fd_store[i] != NULL && fd_store[i]->inode->ino == ino)
    {
      *fd = fd_store[i];
      return 0;
    }
  }
    
  return -1;
}