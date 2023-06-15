#include <string.h>
#include "../../include/operations/fs_read.h"

int fs_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi)
{
  // Implement your logic here to read 'size' bytes from the file specified by 'path' into 'buf'.
  // Start reading from the position specified by 'offset'.
  // For the purpose of this example, let's assume that all files contain the string "Hello, World!".

  const char *hello_str = "Hello, World!";
  size_t len = strlen(hello_str);

  if (offset < len)
  {
    if (offset + size > len)
    {
      size = len - offset;
    }
    memcpy(buf, hello_str + offset, size);
  }
  else
  {
    size = 0;
  }

  return size;
}
