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

// How many bytes are read at a time from the input file
#define BUFFER_SIZE 16

typedef unsigned char b64byte;

// base64_char_to_ascii takes in a base64 character value, and returns its
// ascii representation
unsigned char base64_char_to_ascii(unsigned char input) {
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
  exit(EXIT_FAILURE);
}

// char_to_base64_ascii takes in the ascii value of the base64 character
// and returns the base64 character value
//
// Values from the base64 ascii table https://en.wikipedia.org/wiki/Base64
unsigned char char_to_base64_ascii(unsigned char input) {
  switch (input) {
  case 'A' ... 'Z':
    return input - 'A';
  case 'a' ... 'z':
    return input + BASE64_a - 'a';
  case '0' ... '9':
    return input + BASE64_0 - '0';
  case '+':
    return BASE64_PLUS;
  case '/':
    return BASE64_SLASH;
  }

  printf("Unknown base64 character value for character '%c'\n", input);
  exit(EXIT_FAILURE);
}

#define DISPLAY_LINE_SIZE 64

void base64_encode(FILE *fd) {
  int total_read = 0;

  int total_output = 0;

  unsigned char buffer[BUFFER_SIZE];

  int read = BUFFER_SIZE;

  b64byte current_byte = 0;

  for (int bit_index = 0;; bit_index++) {
    int array_index = bit_index / BYTE_SIZE;

    if (bit_index > 0 && bit_index % BASE64_BYTE_SIZE == 0) {
      printf("%c", base64_char_to_ascii(current_byte));
      total_output++;
      if (total_output % DISPLAY_LINE_SIZE == 0) {
        printf("\n");
      }
      current_byte = 0;
    }

    if (array_index >= total_read) {
      read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fd);
      if (ferror(fd)) {
        perror("Error reading input file");
        exit(EXIT_FAILURE);
      }
      if (read == 0) {
        if (bit_index % BASE64_BYTE_SIZE != 0) {
          printf("%c", base64_char_to_ascii(current_byte));
          total_output++;
          if (total_output % DISPLAY_LINE_SIZE == 0) {
            printf("\n");
          }
        }
        break;
      }
      total_read += read;
    }

    int actual_index = array_index % BUFFER_SIZE;
    int buf_bit_index = 0x01 << ((BYTE_SIZE - 1) - (bit_index % BYTE_SIZE));
    // printf("%d", (buffer[actual_index] & buf_bit_index) != 0);
    int bit_value = (buffer[actual_index] & buf_bit_index) != 0;
    current_byte |=
        bit_value << ((BASE64_BYTE_SIZE - 1) - (bit_index % BASE64_BYTE_SIZE));
  }

  int output_size = (total_read * BYTE_SIZE) / BASE64_BYTE_SIZE + 1;

  if (total_read % 3 != 0) {
    while (output_size % 4 != 0) {
      printf("%c", BASE64_PADDING_CHARACTER);
      total_output++;
      if (total_output % DISPLAY_LINE_SIZE == 0) {
        printf("\n");
      }
      output_size++;
    }
  }

  if (total_output % DISPLAY_LINE_SIZE != 0) {
    printf("\n");
  }
}

void base64_decode(FILE *fd) {
  int total_read = 0;

  unsigned char buffer[BUFFER_SIZE];

  unsigned char current_byte = 0;

  for (int bit_index = 0;; bit_index++) {
    int array_index = (bit_index / BASE64_BYTE_SIZE);

    if (bit_index > 0 && bit_index % BYTE_SIZE == 0) {
      // TODO: Handle padding here becase partial bytes are possible
      // There are two cases:
      // 1. There is a padding byte following. In this case the offset needs to
      // be subtracted
      // 2. There is no byte following, in this case, the file might be invalid
      printf("%c", current_byte);
      current_byte = 0;
    }

    if (array_index >= total_read) {
      int read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fd);
      if (ferror(fd)) {
        perror("Error reading input file");
        exit(EXIT_FAILURE);
      }
      // printf("Read %d bytes\n", read);
      if (read == 0) {
        break;
      }
      total_read += read;
    }

    int actual_index = array_index % BUFFER_SIZE;

    // Technically just ignores all newlines, but still works for now
    if (buffer[actual_index] == '\n') {
      continue;
    }

    if (buffer[actual_index] == BASE64_PADDING_CHARACTER) {
      printf("Padding!");
      break;
    }

    int extracted_bit =
        0x01 << ((BASE64_BYTE_SIZE - 1) - (bit_index % BASE64_BYTE_SIZE));
    // printf("Character %d is [%c], decoded %.6b\n", array_index,
    //        buffer[actual_index], char_to_base64_ascii(buffer[actual_index]));
    int bit_value =
        (char_to_base64_ascii(buffer[actual_index]) & extracted_bit) != 0;
    // printf("Extract bit is %x, data: %d\n", extracted_bit, bit_value);

    // printf("Bit at index %d is %d\n", bit_index, bit_value);

    current_byte |= bit_value << ((BYTE_SIZE - 1) - (bit_index % BYTE_SIZE));
  }
}
