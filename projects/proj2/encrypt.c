#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bit_writer.h"
#include "pbm.h"
#include "stream_cipher.h"

#define BIT_ZERO 0
#define BIT_ONE 1

// https://stackoverflow.com/questions/1068849/how-do-i-determine-the-number-of-digits-of-an-integer-in-c
int num_digits(int n) {
  if (n < 0)
    return num_digits((n == INT_MIN) ? INT_MAX : -n);
  if (n < 10)
    return 1;
  return 1 + num_digits(n / 10);
}

void encrypt_pbm_file(const struct pbm_file *file,
                      struct stream_cipher *cipher) {
  int pbm_byte_width = pbm_width_bytes(file);

  // The outputs of the shares
  struct bit_writer *share1_data =
      init_bit_writer(pbm_byte_width * file->height);
  struct bit_writer *share2_data =
      init_bit_writer(pbm_byte_width * file->height);

  for (int ri = 0; ri < file->height; ri++) {

    // The temporary buffers for the shares
    struct bit_writer *share1_line1 = init_bit_writer(pbm_byte_width);
    struct bit_writer *share1_line2 = init_bit_writer(pbm_byte_width);

    struct bit_writer *share2_line1 = init_bit_writer(pbm_byte_width);
    struct bit_writer *share2_line2 = init_bit_writer(pbm_byte_width);

    for (int ci = 0; ci < pbm_width_bytes(file); ci++) {
      int limit = 8;
      if (ci >= (file->width / 8)) {
        limit = file->width % 8;
      }
      unsigned char current_byte = file->rows[ri][ci];

      // Now that we've extracted a bit from the pbm file, we need to encrypt it
      for (int bit_index = 0; bit_index < limit; bit_index++) {
        int bit_set = (current_byte & (0x01 << (7 - bit_index))) != 0;

        int random_bit = stream_cipher_get_random_bit(cipher);

        switch (bit_set) {
        case BIT_ONE: {
          switch (random_bit) {
          case 0: {
            bit_writer_write_bit(share1_line1, 0);
            bit_writer_write_bit(share1_line1, 1);
            bit_writer_write_bit(share1_line2, 1);
            bit_writer_write_bit(share1_line2, 0);

            bit_writer_write_bit(share2_line1, 0);
            bit_writer_write_bit(share2_line1, 1);
            bit_writer_write_bit(share2_line2, 1);
            bit_writer_write_bit(share2_line2, 0);
            break;
          }
          case 1: {
            bit_writer_write_bit(share1_line1, 1);
            bit_writer_write_bit(share1_line1, 0);
            bit_writer_write_bit(share1_line2, 0);
            bit_writer_write_bit(share1_line2, 1);

            bit_writer_write_bit(share2_line1, 1);
            bit_writer_write_bit(share2_line1, 0);
            bit_writer_write_bit(share2_line2, 0);
            bit_writer_write_bit(share2_line2, 1);
            break;
          }
          }
          break;
        }
        case BIT_ZERO:
          switch (random_bit) {
          case 0: {
            bit_writer_write_bit(share1_line1, 0);
            bit_writer_write_bit(share1_line1, 1);
            bit_writer_write_bit(share1_line2, 1);
            bit_writer_write_bit(share1_line2, 0);

            bit_writer_write_bit(share2_line1, 1);
            bit_writer_write_bit(share2_line1, 0);
            bit_writer_write_bit(share2_line2, 0);
            bit_writer_write_bit(share2_line2, 1);
            break;
          }
          case 1: {
            bit_writer_write_bit(share1_line1, 1);
            bit_writer_write_bit(share1_line1, 0);
            bit_writer_write_bit(share1_line2, 0);
            bit_writer_write_bit(share1_line2, 1);

            bit_writer_write_bit(share2_line1, 0);
            bit_writer_write_bit(share2_line1, 1);
            bit_writer_write_bit(share2_line2, 1);
            bit_writer_write_bit(share2_line2, 0);
            break;
          }
          }
          break;
        }
      }
    }

    // Once the line is finished, write it out to the shares
    bit_writer_write_bytes_unaligned(share1_data, share1_line1->data,
                                     share1_line1->capacity);
    bit_writer_write_bytes_unaligned(share1_data, share1_line2->data,
                                     share1_line2->capacity);
    bit_writer_write_bytes_unaligned(share2_data, share2_line1->data,
                                     share2_line1->capacity);
    bit_writer_write_bytes_unaligned(share2_data, share2_line2->data,
                                     share2_line2->capacity);

    // Clean up all the bit writers for the lines
    bit_writer_close(share1_line1);
    bit_writer_close(share1_line2);
    bit_writer_close(share2_line1);
    bit_writer_close(share2_line2);
    free(share1_line1);
    free(share1_line2);
    free(share2_line1);
    free(share2_line2);
  }

  // Finally, write out the shares to valid PBM files
  int share_width = file->width * 2;
  int share_height = file->height * 2;
  // Two more bits for the space between the numbers, as well as the newline
  int share_size_bytes = (pbm_byte_width * file->height) +
                         PBM_MAGIC_NUMBER_BYTES + num_digits(share_width) +
                         num_digits(share_height) + 2;
  unsigned char *share1_pbm_data =
      malloc(sizeof(unsigned char) * share_size_bytes);
  unsigned char *share2_pbm_data =
      malloc(sizeof(unsigned char) * share_size_bytes);

  // Write out the PBM file formats
  sprintf((char *)share1_pbm_data, "%s\n%d %d\n%s", PBM_MAGIC_NUMBER,
          share_width, share_height, share1_data->data);
  sprintf((char *)share2_pbm_data, "%s\n%d %d\n%s", PBM_MAGIC_NUMBER,
          share_width, share_height, share2_data->data);

  // Make sure that the copying was done correctly
  assert((int)strlen((char *)share1_pbm_data) == share_size_bytes);
  assert((int)strlen((char *)share2_pbm_data) == share_size_bytes);

  struct pbm_file *share1 =
      read_pbm_file((char *)share1_pbm_data, share_size_bytes);
  struct pbm_file *share2 =
      read_pbm_file((char *)share2_pbm_data, share_size_bytes);

  free(share1_pbm_data);
  free(share2_pbm_data);

  FILE *share1_file = fopen("share1", "ab+");
  if (share1_file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  pbm_file_write_to_file(share1, share1_file);
  if (ferror(share1_file)) {
    perror("Error writing to PBM output file");
    exit(EXIT_FAILURE);
  }
  fclose(share1_file);

  FILE *share2_file = fopen("share2", "ab+");
  if (share2_file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  pbm_file_write_to_file(share2, share2_file);
  if (ferror(share2_file)) {
    perror("Error writing to PBM output file");
    exit(EXIT_FAILURE);
  }
  fclose(share2_file);

  pbm_file_close(share1);
  free(share1);
  pbm_file_close(share2);
  free(share2);
}
