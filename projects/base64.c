#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

#define BYTE_SIZE 8
#define BASE64_BYTE_SIZE 6
#define BASE64_PADDING_CHARACTER '='
#define BASE64_ALIGNMENT_AMOUNT 4

// From the Base64 ascii table
// https://en.wikipedia.org/wiki/Base64
#define BASE64_A 0
#define BASE64_Z 25

#define BASE64_a 26
#define BASE64_z 51

#define BASE64_0 52
#define BASE64_9 61

#define BASE64_PLUS 62
#define BASE64_SLASH 63

unsigned char base64_ascii_to_char(char input) {
  // Using case ranges
  // https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html
  switch (input) {
  case BASE64_A ... BASE64_Z:
    return input + 'A';
  case BASE64_a ... BASE64_z:
    return (input + 'a') - BASE64_a;
  case BASE64_0 ... BASE64_9:
    return (input + '0') - BASE64_0;
  case BASE64_PLUS:
    return '+';
  case BASE64_SLASH:
    return '/';
  }
  printf("Unknown base64 conversion from character [%c]\n", input);
  return 0;
}

char *base64_encode(const char *data, int data_size) {
  int num_bits = sizeof(char) * data_size * BYTE_SIZE;
  int num_blocks = num_bits / BASE64_BYTE_SIZE;

  bool needs_padding = num_bits % BASE64_BYTE_SIZE != 0;
  // Division always rounds down, so if we need padding, we're always going to
  // be one block short
  if (needs_padding) {
    num_blocks += 1;
  }

  // base64 is always padded until its block length is a multiple of four
  int extra_padding =
      BASE64_ALIGNMENT_AMOUNT - (num_blocks % BASE64_ALIGNMENT_AMOUNT);

  // Create an array to store the blocks
  // We don't know the size of this until we calculate it, so it has to live
  // on the heap
  char *output_str = malloc(sizeof(char) * (num_blocks + extra_padding));
  int output_index = 0;
  if (output_str == NULL) {
    printf("Allocation of array failed!\n");
    return NULL;
  }

  char temp_byte = 0;

  // Loop through each bit in the input, and fill `temp_byte` with the first 6
  // bits
  for (int i = 0; i < num_bits; i++) {
    // This is the array index that the current bit is in
    int data_index = i / BYTE_SIZE;
    // `reverse_bit` extracts the ith bit of the current byte, starting from the
    // most significant bit
    int reverse_bit =
        (data[data_index] & (0x01 << ((BYTE_SIZE - 1) - (i % BYTE_SIZE)))) != 0;
    // `temp_shift` is the index of the bit in the base64 byte
    int temp_shift = (BASE64_BYTE_SIZE - 1) - (i % BASE64_BYTE_SIZE);
    // Set that bit to the extracted bit
    temp_byte |= reverse_bit << temp_shift;
    // Once we've filled up a full base64 byte, add it to the array
    if (i % BASE64_BYTE_SIZE == (BASE64_BYTE_SIZE - 1)) {
      output_str[output_index++] = base64_ascii_to_char(temp_byte);
      // Reset it as well
      temp_byte = 0;
    }
  }

  // If the result was padded, then we'll need to treat the in-progress base64
  // byte as a full byte, and it's already zero-padded
  if (needs_padding) {
    output_str[output_index] = base64_ascii_to_char(temp_byte);
  }

  // Add padding
  for (int i = 0; i < extra_padding; i++) {
    output_str[output_index + extra_padding] = BASE64_PADDING_CHARACTER;
  }

  return output_str;
}
