#ifndef FS_BPLUS_TREE_NODE_H
#define FS_BPLUS_TREE_NODE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct bplus_tree_node {
  bool is_leaf;             // flag to indicate if this node is a leaf node
  uint64_t* keys;           // array of keys
  void** pointers;          // array of pointers
} bplus_tree_node_t;

bplus_tree_node_t* create_bplus_tree_node(bool is_leaf);
void delete_bplus_tree_node(bplus_tree_node_t* node);
int insert_into_bplus_tree(bplus_tree_node_t* root, uint64_t key, void* pointer);
void* search_in_bplus_tree(bplus_tree_node_t* root, uint64_t key);
int remove_from_bplus_tree(bplus_tree_node_t* root, uint64_t key);


#endif // FS_BPLUS_TREE_NODE_H
