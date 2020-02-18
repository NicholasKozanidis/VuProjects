#include <stdio.h> // Include standard I/O header

// main is called on start, with the number of command
// line arguments stored in argc, and the pointers to the
// arguments in argv. argv is an array of char pointers.
int main(int argc, char **argv)
{

  char *binary_name = argv[0];

  if (argc < 2) {
    printf("expected 2 command line arguments, got %d\n", argc - 1);
    return 1; // return status code 1 for error
  }

  char *name = argv[1];
  char *code = argv[2];

  printf("Hello %s, you entered the code %s\n", name, code)

  return 0; // return status code 0 for normal execution
}
