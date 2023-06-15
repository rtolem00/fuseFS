#include <cache/backends/lru_cache.h>
#include <storage/backends/ondisk_backend.h>

#include <inode/fs_inode.h>
#include <structures/fs_dir_entry.h>

#include <sys/stat.h>


// Other includes...

#define ROOT_DIR_NAME "/"

void initialize_filesystem(char* storage_path)
{
  // Initialize the storage backend
  storage_backend_t* disk_backend = create_disk_backend(storage_path);
  set_storage_backend(disk_backend);

  // Initialize the cache
  init_lru_cache();
  cache_t* lru_cache = create_cache(lru_add_block, lru_get_block);
  set_cache_backend(lru_cache);

  // Create root directory and its inode
  inode_t root_inode;
  root_inode.ino = 0;  // inode number for root is always 0
  root_inode.mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;  // drwxrwxr-x
  root_inode.nlink = 1;  // '.' directory reference to itself
  root_inode.uid = getuid();
  root_inode.gid = getgid();
  root_inode.size = sizeof(struct dir_entry);
  root_inode.atime = root_inode.mtime = root_inode.ctime = time(NULL);
  root_inode.block_count = 1;
  root_inode.blocks = malloc(sizeof(uint64_t));
  root_inode.blocks[0] = find_free_block();
  mark_block_as_used(root_inode.blocks[0]);
  write_inode(root_inode);

  // Create root directory entry
  dir_entry_t root_dir_entry;
  strcpy(root_dir_entry.filename, ROOT_DIR_NAME);
  root_dir_entry.ino = root_inode.ino;
  write_dir_entry(root_inode.blocks[0], &root_dir_entry);
    
  // Initialize block bitmap
  initialize_block_bitmap();
}
