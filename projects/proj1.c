#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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
    printf("Unknown base64 conversion from character %c\n", input);
    return 0;
  }
}

int main(void) {
  char* input = "Hi\n";
  int input_len = strlen(input);

  int num_bits = sizeof(char) * input_len * 8;

  int num_blocks = num_bits / 6;
  bool needs_padding = num_bits % 6 != 0;
  if (needs_padding) {
    // Block needs to be padded, add an extra block
    num_blocks += 1;
  }

  char* output_str = malloc(sizeof(char) * num_blocks);
  int output_index = 0;
  if (output_str == NULL) {
    printf("Allocation of array failed!\n");
    return 1;
  }

  char temp_byte = 0;

  for (int i = 0; i < num_bits; i++) {
    int data_index = i / 8;
    int reverse_bit = (input[data_index] & (0x01 << (7 - (i % 8)))) != 0;
    int temp_shift = 5 - (i % 6);
    temp_byte |= reverse_bit << temp_shift;
    if (i % 6 == 5) {
      output_str[output_index++] = temp_byte;
      temp_byte = 0;
    }
  }

  if (needs_padding) {
    output_str[output_index] = temp_byte;
  }

  for (int bi = 0; bi < num_blocks; bi++) {
    //printf("%.6b ", output_str[bi]);
    printf("%c", base64_ascii_to_char(output_str[bi]));
  }

  // Pad out the rest
  while (num_blocks++ % 4 != 0) {
    //printf("%.6b ", 0x0);
    printf("=");
  }

  printf("\n");

  free(output_str);
  return 0;
}
