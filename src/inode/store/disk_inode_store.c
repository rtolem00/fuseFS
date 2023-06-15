// disk_inode_store.c
#include <inode/inode_store.h>
#include <bitmap/inode_bitmap.h>
#include <stdio.h>
#include <stdlib.h>


static FILE* file = NULL;

static int disk_store_inode(inode_t* inode)
{
  // Write the inode fields
  fseek(file, inode->ino * sizeof(inode_t), SEEK_SET);
  fwrite(inode, sizeof(inode_t), 1, file);
    
  // Write the block numbers separately
  fwrite(inode->blocks, sizeof(uint64_t), inode->block_count, file);
    
  fflush(file);
  fclose(file);

  return 0;
}

static inode_t* disk_retrieve_inode(uint64_t ino)
{
  inode_t* inode = malloc(sizeof(inode_t));

  fseek(file, ino * sizeof(inode_t), SEEK_SET);
  fread(inode, sizeof(inode_t), 1, file);

  // Allocate memory for block numbers and read them
  inode->blocks = malloc(sizeof(uint64_t) * inode->block_count);
  fread(inode->blocks, sizeof(uint64_t), inode->block_count, file);

  return inode;
}

static int disk_update_inode(inode_t* inode)
{
  // Just store the updated inode
  return disk_store_inode(inode);
}

static int disk_delete_inode(uint64_t ino)
{
  // Clear the inode data
  inode_t empty_inode;
  memset(&empty_inode, 0, sizeof(inode_t));

  fseek(file, ino * sizeof(inode_t), SEEK_SET);
  fwrite(&empty_inode, sizeof(inode_t), 1, file);
  fflush(file);

  return 0;
}

inode_store_t* create_disk_inode_store(const char* inode_file)
{
  file = fopen(inode_file, "r+b");
  if (!file)
  {
    return NULL;
  }

  inode_store_t* store = create_inode_store(disk_store_inode, disk_retrieve_inode, disk_update_inode, disk_delete_inode);
  if (!store)
  {
    fclose(file);
    return NULL;
  }

  return store;
}

void destroy_disk_inode_store()
{
  fclose(file);
  free_inode_store();
}
