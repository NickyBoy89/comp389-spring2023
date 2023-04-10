#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "hexdump.h"

#define HEXDUMP_COMMAND "hexdump"
#define ENCODE_COMMAND "enc-base64"
#define DECODE_COMMAND "dec-base64"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("ERROR: No command specified\n");
    return 0;
  }

  if (argc < 3) {
    printf("ERROR: No files specified to read\n");
    return 1;
  } else if (argc > 3) {
    printf("WARN: Too many arguments specified, only reading the first "
           "file\n");
  }

  char *input_filename = argv[2];

  FILE *fd = fopen(input_filename, "rb");
  if (fd == NULL) {
    printf("Filename: %s\n", input_filename);
    perror("Error accessing file");
    return 1;
  }

  // Get the size of the file in bytes
  fseek(fd, 0, SEEK_END);
  int file_size = ftell(fd);
  rewind(fd);

#define BYTES_PER_ROW 16
#define HEXDUMP_DATA_SIZE 16
#define BUFFER_SIZE 16

  if (strcmp(argv[1], HEXDUMP_COMMAND) == 0) {
    int total_read = 0;
    unsigned char line_buffer[HEXDUMP_DATA_SIZE];
    while (total_read < file_size) {
      int read =
          fread(line_buffer, sizeof(unsigned char), HEXDUMP_DATA_SIZE, fd);
      total_read += read;
      if (ferror(fd)) {
        perror("Error reading data from file");
        fclose(fd);
        return 1;
      }
      hexdump_data(line_buffer, HEXDUMP_DATA_SIZE, read);
    }
    if (total_read != file_size) {
      printf("ERROR: Read %d bytes, but expected %d\n", total_read, file_size);
      fclose(fd);
      return 1;
    }
  } else if (strcmp(argv[1], ENCODE_COMMAND) == 0) {
    base64_encode(fd, file_size);
  } else if (strcmp(argv[1], DECODE_COMMAND) == 0) {
    base64_decode(fd, file_size);
    return 1;
  }

  fclose(fd);

  return 0;
}
