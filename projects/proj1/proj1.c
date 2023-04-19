#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "hexdump.h"

#define HEXDUMP_COMMAND "hexdump"
#define ENCODE_COMMAND "enc-base64"
#define DECODE_COMMAND "dec-base64"

// Hexdump spacing
#define BYTES_PER_ROW 16
#define HEXDUMP_DATA_SIZE 16
#define BUFFER_SIZE 16

enum command_options {
  HEXDUMP,
  BASE64_ENCODE,
  BASE64_DECODE,
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("ERROR: No command specified\n");
    return EXIT_FAILURE;
  }

  enum command_options command_type;

  if (strcmp(argv[1], HEXDUMP_COMMAND) == 0) {
    command_type = HEXDUMP;
  } else if (strcmp(argv[1], ENCODE_COMMAND) == 0) {
    command_type = BASE64_ENCODE;
  } else if (strcmp(argv[1], DECODE_COMMAND) == 0) {
    command_type = BASE64_DECODE;
  } else {
    printf("ERROR: Invalid command specified: \"%s\"\n", argv[1]);
    return EXIT_FAILURE;
  }

  int read_from_stdin = false;

  if (argc < 3) {
    read_from_stdin = true;
  } else if (argc > 3) {
    printf("WARN: Too many arguments specified, only reading the first "
           "file\n");
  }

  FILE *fd;

  if (read_from_stdin) {
    fd = stdin;
  } else {
    char *input_filename = argv[2];

    fd = fopen(input_filename, "rb");
    if (fd == NULL) {
      printf("Filename: %s\n", input_filename);
      perror("Error accessing file");
      return 1;
    }
  }

  switch (command_type) {
  case HEXDUMP:
    hexdump_data(fd);
    break;
  case BASE64_ENCODE:
    base64_encode(fd);
    break;
  case BASE64_DECODE:
    base64_decode(fd);
    break;
  }

  fclose(fd);

  return 0;
}
