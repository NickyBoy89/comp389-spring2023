#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_SIZE 8
#define BASE64_BYTE_SIZE 6

unsigned char base64_ascii_to_char(char input) {
  if (input >= 0 && input <= 25) {
    return input + 'A';
  } else if (input >= 26 && input <= 51) {
    return input + 'a' - 26;
  } else if (input >= 52 && input <= 61) {
    return input + '0' - 52;
  } else if (input == 62) {
    return '+';
  } else if (input == 63) {
    return '/';
  } else {
    printf("Unknown base64 conversion from character [%c]\n", input);
    return 0;
  }
}

void hexdump_data(const char data[], int data_size) {
  int num_columns = data_size / 16;
  if (num_columns % 8 != 0) {
    num_columns += 1;
  }

  for (int ri = 0; ri < num_columns; ri++) {
    int start_index = 2 * ri;
    printf("Column %d starts at index %d and ends at %d\n", ri, start_index, 0);
  }
}

int main(void) {
  char *input = "Hi\n";
  int input_len = strlen(input);

  int num_bits = sizeof(char) * input_len * 8;
  int num_blocks = num_bits / BASE64_BYTE_SIZE;

  bool needs_padding = num_bits % BASE64_BYTE_SIZE != 0;
  // Division always rounds down, so if we need padding, we're always going to
  // be one block short
  if (needs_padding) {
    num_blocks += 1;
  }

  // Create an array to store the blocks
  // We don't know the size of this until we calculate it, so it has to live
  // on the heap
  char *output_str = malloc(sizeof(char) * num_blocks);
  int output_index = 0;
  if (output_str == NULL) {
    printf("Allocation of array failed!\n");
    return 1;
  }

  char temp_byte = 0;

  // Loop through each bit in the input
  for (int i = 0; i < num_bits; i++) {
    // This is the array index that the current bit is in
    int data_index = i / BYTE_SIZE;
    // `reverse_bit` extracts the ith bit of the current byte, starting from the
    // most significant bit
    int reverse_bit = (input[data_index] &
                       (0x01 << ((BYTE_SIZE - 1) - (i % BYTE_SIZE)))) != 0;
    // `temp_shift` is the index of the bit in the base64 byte
    int temp_shift = (BASE64_BYTE_SIZE - 1) - (i % BASE64_BYTE_SIZE);
    // Set that bit to the extracted bit
    temp_byte |= reverse_bit << temp_shift;
    // Once we've filled up a full base64 byte, add it to the array
    if (i % BASE64_BYTE_SIZE == (BASE64_BYTE_SIZE - 1)) {
      output_str[output_index++] = temp_byte;
      // Reset it as well
      temp_byte = 0;
    }
  }

  // If the result was padded, then we'll need to treat the in-progress base64
  // byte as a full byte, and it's already zero-padded
  if (needs_padding) {
    output_str[output_index] = temp_byte;
  }

  // Print out the character encoding of every base64 byte
  for (int bi = 0; bi < num_blocks; bi++) {
    // printf("%.6b ", output_str[bi]);
    printf("%c", base64_ascii_to_char(output_str[bi]));
  }

  // Pad the rest until the block length is a multiple of 4
  while (num_blocks++ % 4 != 0) {
    // printf("%.6b ", 0x0);
    printf("=");
  }

  printf("\n");

  free(output_str);
  return 0;
}
