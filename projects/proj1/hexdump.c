#include <stdio.h>

#define BYTES_PER_ROW 16

#if BYTES_PER_ROW % 2 == 0
#define BYTES_HALFWAY BYTES_PER_ROW / 2
#else
#error "Bytes per row is not a power of two"
#endif

#define HEXDUMP_DATA_SIZE 16

#define FILLER_CHARACTER '.'
#define INVALID_CHARACTER '~'

void hexdump_data(const unsigned char data[], int data_size, int start_byte) {
  int row_counter = start_byte;
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
        printf(" ");
      }
    }

    // Put just once space because all the hex values have a space after them
    printf(" ");

    // Print the string representation of the bits
    for (int current_byte = 0; current_byte < BYTES_PER_ROW; current_byte++) {
      char character = data[row + current_byte];

      // Don't print out the padding bytes
      if (current_byte >= data_size) {
        printf(" ");
        continue;
      }

      if (character >= ' ' && character <= '~') {
        // Only print out the characters that work
        printf("%c", character);
      } else if (character < 0) {
        printf("%c", INVALID_CHARACTER);
      } else {
        // Print a filler character for everything that can't be printed
        printf("%c", FILLER_CHARACTER);
      }
    }

    // Go to the next line
    printf("\n");
    row_counter += BYTES_PER_ROW;
  }
}
