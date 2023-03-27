#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

#define BYTES_PER_ROW 16
#if BYTES_PER_ROW % 2 == 0
#define BYTES_HALFWAY BYTES_PER_ROW / 2
#else
#error "Bytes per row is not a power of two"
#endif

void hexdump_data(const char data[], int data_size) {
  int row_counter = 0;
  for (int row = 0; row < data_size; row += BYTES_PER_ROW) {
    // Print out the row counter
    printf("%.6x: ", row_counter);

    // Print the hex of every 16 bytes
    for (int current_byte = 0; current_byte < BYTES_PER_ROW; current_byte++) {
      // Replace anything blank with padding characters
      if (row + current_byte >= data_size) {
        printf("-- ");
      } else {
        printf("%.2x ", data[row + current_byte]);
      }

      // Put a space halfway in between both columns
      if (current_byte == (BYTES_HALFWAY - 1)) {
        printf("  ");
      }
    }

    // Put just once space because all the hex values have a space after them
    printf(" ");

    // Print the string representation of the bits
    for (int current_byte = 0; current_byte < BYTES_PER_ROW; current_byte++) {
      char character = data[row + current_byte];

      switch (character) {
        // Only print out the characters that work
      case 'a' ... 'z':
      case 'A' ... 'Z':
        printf("%c", character);
        break;
        // Print a filler character for everything that can't be printed
      default:
        printf(".");
        break;
      }
    }

    // Go to the next line
    printf("\n");
    row_counter += BYTES_PER_ROW;
  }
}

int main(void) {
  char *input = "Hi";
  int input_len = strlen(input);

  FILE *fd = fopen("test1.txt", "rb");
  if (fd == NULL) {
    printf("Error accessing file\n");
    return 1;
  }

  // Get the size of the file in bytes
  fseek(fd, 0, SEEK_END);
  int file_size = ftell(fd);
  rewind(fd);

  char *file_buffer = (char *)malloc(sizeof(char) * file_size);
  if (file_buffer == NULL) {
    printf("Error allocating memory for file\n");
    return 1;
  }

  int bytes_read = fread(file_buffer, 1, file_size, fd);
  if (bytes_read != file_size) {
    printf("Error reading file, tried to read %d bytes, got %d\n", file_size,
           bytes_read);
    return 1;
  }

  fclose(fd);

  hexdump_data(file_buffer, file_size);

  free(file_buffer);

  char *encoded = base64_encode(input, input_len);
  if (encoded == NULL) {
    printf("Error encoding base64 data");
    return 1;
  }

  printf("Base64 encoded %s -> %s\n", input, encoded);
  free(encoded);

  return 0;
}
