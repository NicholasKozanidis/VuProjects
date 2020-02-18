#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

  char *binary_name = argv[0];

  if (argc < 5) {
    // fprintf prints to a FILE stream. In this case the standard error.
    fprintf(stderr, "usage: %s FILE_NAME NUM_CHUNKS CHUNK_SIZE OFFSET\n", argv[0]);
    fprintf(stderr, "Reads NUM_CHUNKS chunks of size CHUNK_SIZE from file FILE_NAME, starting at OFFSET bytes into the file\n");
    return 1; // return status code 1 for error
  }

  char *file_name = argv[1];
  long num_chunks = atol(argv[2]); // atol converts a string to a long
  long chunk_size = atol(argv[3]);
  long offset = atoi(argv[4]);

  FILE *fp = fopen(file_name, "r");

  if (fp == NULL) {
    fprintf(stderr, "The file %s could not be opened\n", file_name);
    return 1;
  }

  if (fseek(fp, offset, 0) != 0) {
    fprintf(stderr, "fseek failed\n");
    return 1;
  }

  // Allocate the buffer
  char *buf = malloc(chunk_size * num_chunks);

  // Read bytes
  size_t bytes_read = fread(buf, chunk_size, num_chunks, fp);

  // Write bytes
  size_t bytes_written = fwrite(buf, chunk_size, num_chunks, stdout);

  // Free the buffer
  free(buf);

  // Print statistics
  fprintf(stderr, "Read %zu bytes and wrote %zu bytes in chunks of %lu\n",
      bytes_read, bytes_written, chunk_size);

  return 0; // return status code 0 for normal execution
}
