#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <unistd.h>

// Import our filesystem operations
#include "../include/operations/fs_getattr.h"
#include "../include/operations/fs_read.h"
// And so on for other operations...

static struct fuse_operations fs_oper = {
  .getattr    = fs_getattr,
  .read       = fs_read,
  //... Add other operations here
};

int main(int argc, char *argv[])
{
  umask(0);
  return fuse_main(argc, argv, &fs_oper, NULL);
}