#include <cache/lru_cache.h>
#include <storage/ondisk_backend.h>
#include <structures/bitmap/disk_bitmap.h>
#include <structures/bitmap/inode_bitmap.h>
#include <structures/bitmap/block_bitmap.h>

#include <inode/inode_store.h>
#include <inode/store/disk_inode_store.h>

#include <fs_inode.h>
#include <structures/fs_dir_entry.h>

#include <structures/bitmap.h>

#include <sys/stat.h>
#include <errno.h> // Added this line
#include <stdio.h>


// Other includes...

#define ROOT_DIR_NAME "/"


#define BLOCK_BITMAP_FILE "block_bitmap"
#define INODE_BITMAP_FILE "inode_bitmap"
#define INODE_TABLE_FILE "inode_table"

inode_t* root_dir = NULL;


void initialize_filesystem(char* storage_path, uint64_t max_blocks)
{
  // Initialize the storage backend
  storage_backend_t* disk_backend = create_disk_backend(storage_path);
  set_storage_backend(disk_backend);

  // Initialize the cache
  init_lru_cache();
  cache_t* lru_cache = create_cache(lru_add_block, lru_get_block);
  set_cache_backend(lru_cache);

  // Initialize the block bitmap
  bitmap_t* block_bitmap = create_disk_bitmap(BLOCK_BITMAP_FILE);
  if (block_bitmap == NULL)
  {
    fprintf(stderr, "Failed to create block bitmap\n");
    return -ENOMEM;
  }

  // Initialize the inode bitmap
  bitmap_t* inode_bitmap = create_disk_bitmap(INODE_BITMAP_FILE);
  if (inode_bitmap == NULL)
  {
    fprintf(stderr, "Failed to create inode bitmap\n");
    free_bitmap(block_bitmap);
    return -ENOMEM;
  }

  // Initialize the inode store
  inode_store_t* inode_store = create_disk_inode_store(INODE_TABLE_FILE);
  if (inode_store == NULL)
  {
    fprintf(stderr, "Failed to create inode store\n");
    free_bitmap(block_bitmap);
    free_bitmap(inode_bitmap);
    return -ENOMEM;
  }
  set_inode_store(inode_store);

  // Initialize the root directory
  if (initialize_root_directory() != 0)
  {
    fprintf(stderr, "Failed to initialize root directory: %s\n", strerror(-1));
    free_bitmap(block_bitmap);
    free_bitmap(inode_bitmap);
    return -ENOMEM;
  }
}

int initialize_root_directory()
{
  if (create_root_dir(ROOT_DIR_NAME) != 0)
  {
    fprintf(stderr, "Failed to create root directory\n");
    return -ENOMEM;
  }

  return 0;
}
