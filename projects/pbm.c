#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pbm.h"

#define MAX_WHITESPACE 3
#define MAGIC_NUMBER "P4"

struct pbm_file* read_pbm_file(const char data[], int data_size) {
  struct pbm_file* file = malloc(sizeof(struct pbm_file));

  // There are three whitespace characters:
  // 1. After the magic number and before the width
  // 2. After the width and before the height
  // 3. After the height and before the 
  int indexes[MAX_WHITESPACE] = { 0 };
  int num_indexes = 0;
  for (int i = 0; i < data_size; i++) {
    switch (data[i]) {
      case ' ':
      case '\n':
        if (num_indexes > MAX_WHITESPACE) {
          printf("More whitespace than normal detected\n");
          return NULL;
        }
        indexes[num_indexes++] = i;
        break;
    }
  }

  if (num_indexes != MAX_WHITESPACE) {
    printf("Wrong number of whitespace characters found, expected %d, got %d\n",
       MAX_WHITESPACE,
       num_indexes
    );
    return NULL;
  }

  char* magic_temp = strndup(&data[0], indexes[0]);
  if (strcmp(magic_temp, MAGIC_NUMBER) != 0) {
    printf("Invalid magic number [%s]\n", magic_temp);
    free(magic_temp);
    return NULL;
  }
  free(magic_temp);

  char* width_temp = strndup(&data[indexes[0] + 1], indexes[1]);
  int width = atoi(width_temp);
  free(width_temp);

  char* height_temp = strndup(&data[indexes[1] + 1], indexes[2]);
  int height = atoi(height_temp);
  free(height_temp);

  file->width = width;
  file->height = height;

  int width_bytes = width / 8;
  if (width % 8 != 0) {
    width_bytes += 1;
  }

  int padded_width_bytes = (width / 8) + 1;

  int num_bytes = 0;
  if (width % 8 == 0) {
    num_bytes = (width * height) / 8;
  } else {
    num_bytes = height * padded_width_bytes;
  }

  int input_size = (data_size) - 1 - indexes[2];
  if (input_size != num_bytes) {
    printf("Expected %d bytes of data, got %d bytes\n", num_bytes, input_size);
    return NULL;
  }

  int current_index = indexes[2] + 1;

  char** rows = malloc(sizeof(char*) * height);
  for (int ri = 0; ri < height; ri++) {
    rows[ri] = strndup(&data[current_index], width_bytes);
    current_index += width_bytes;
  }

  file->rows = rows;

  return file;
}

void print_bits_of_char(unsigned char c, int limit) {
  int num_bits = sizeof(unsigned char) * 8;
  for (int i = 0; i < num_bits; i++) {
    if (i >= limit) {
      break;
    }
    int bit_index = (num_bits - 1) - i;
    int bit_value = (c & (0x01 << bit_index)) != 0;
    switch (bit_value) {
      case 0:
        printf(" ");
        break;
      case 1:
        printf("#");
        break;
    }
  }
}

int padded_pbm_width(const struct pbm_file* file) {
  if (file->width % 8 == 0) {
    return file->width / 8;
  }
  return (file->width / 8) + 1;
}

void display_pbm_file(const struct pbm_file* file) {
  for (int ri = 0; ri < file->height; ri++) {
    for (int ci = 0; ci < padded_pbm_width(file); ci++) {
      int limit = 8;
      if (ci >= (file->width / 8)) {
        limit = file->width % 8;
      }
      print_bits_of_char((unsigned char)file->rows[ri][ci], limit);
    }
    printf("\n");
  }
}

void close_pbm_file(struct pbm_file* file) {
  for (int i = 0; i < file->height; i++) {
    free(file->rows[i]);
  }
  free(file->rows);
}
