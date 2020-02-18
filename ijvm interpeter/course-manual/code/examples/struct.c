#include <stdio.h> // Include standard I/O header

typedef struct {
  int size;
  int tag;
  char *buffer;
} my_struct_t;

#define MEM_SIZE 42
#define BLOCK_SIZE 24

// Only writes to block if size is smaller than the size of the block, and I
// have the right permissions
int safe_write(my_struct_t *block, char *buffer, int size, int permission_tag)
{
  if (block->size < size) {
    return -1;
  }
  if (block->tag != permission_tag) {
    return -1;
  }
  if (buffer == NULL) {
    return -1;
  }
  return memcpy(block->buffer, buffer, size);
}

int main(int argc, char **argv)
{

  my_struct_t mymemory[MEM_SIZE];

  for (int i = 0; i < MEM_SIZE; i++) {
    // Initialize values to 0
    memset(&mymemory[i], '\0', sizeof(my_struct_t));

    mymemory[i].size = BLOCK_SIZE;
    mymemory[i].buffer = malloc(BLOCK_SIZE);
    mymemory[i].tag = 0x42;
  }

  return 0; // return status code 0 for normal execution
}
