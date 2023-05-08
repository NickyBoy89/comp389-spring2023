#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bit_writer.h"

struct bit_writer *init_bit_writer(int capacity) {
  struct bit_writer *writer = malloc(sizeof(struct bit_writer));
  writer->written = 0;
  writer->capacity = capacity;
  writer->data = malloc(sizeof(unsigned char) * capacity);
  // Zero out the array
  memset(writer->data, 0, sizeof(unsigned char) * capacity);
  return writer;
}

void bit_writer_print_data(const struct bit_writer *writer) {
  for (int i = 0; i < writer->capacity; i++) {
    printf("%.8b ", writer->data[i]);
  }
  printf("\n");
}

void bit_writer_write_bit(struct bit_writer *writer, int bit_value) {
  // Find the byte that we want to write to
  int byte_array_index = writer->written / 8;
  if (byte_array_index >= writer->capacity) {
    printf("ERROR: Out-of-bounds write on bit writer");
    exit(EXIT_FAILURE);
  }

  // Now, we need to find the position in the byte that we need to write to
  int bit_index = writer->written % 8;
  // Write to the bit, since it is already zeroed
  writer->data[byte_array_index] |= (bit_value << (7 - bit_index));
  writer->written++;
}

void bit_writer_write_bytes_unaligned(struct bit_writer *writer,
                                      unsigned char *bytes, int num_written) {
  int start_index = writer->written / 8;
  if (start_index + num_written >= writer->capacity) {
    printf("ERROR: Writing bytes would write out-of-bounds");
    exit(EXIT_FAILURE);
  }
  memcpy(&writer->data[start_index], bytes, num_written);
  writer->written += num_written * 8;
}

void bit_writer_reset(struct bit_writer *writer) {
  free(writer->data);
  writer->data = malloc(sizeof(unsigned char) * writer->capacity);
  memset(writer->data, 0, sizeof(unsigned char) * writer->capacity);
  writer->written = 0;
}

void bit_writer_close(struct bit_writer *writer) {
  free(writer->data);
  writer->written = 0;
}
