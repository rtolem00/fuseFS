#include <inode/inode_store.h>


// Singleton instance of the inode store
static inode_store_t* inode_store;

inode_store_t* create_inode_store(int (*store_inode)(inode_t*), inode_t* (*retrieve_inode)(uint64_t), int (*update_inode)(inode_t*), int (*delete_inode)(uint64_t))
{
  inode_store_t* store = malloc(sizeof(inode_store_t));
  if(store == NULL)
  {
    return NULL;
  }
  store->store_inode = store_inode;
  store->retrieve_inode = retrieve_inode;
  store->update_inode = update_inode;
  store->delete_inode = delete_inode;
  return store;
}

void set_inode_store(inode_store_t* store)
{
  inode_store = store;
}

int store_inode_to_store(inode_t* inode)
{
  return inode_store->store_inode(inode);
}

inode_t* retrieve_inode_from_store(uint64_t ino)
{
  return inode_store->retrieve_inode(ino);
}

int update_inode_in_store(inode_t* inode)
{
  return inode_store->update_inode(inode);
}

int delete_inode_from_store(uint64_t ino)
{
  return inode_store->delete_inode(ino);
}

int free_inode_store()
{
  free(inode_store);
  return 0;
}

