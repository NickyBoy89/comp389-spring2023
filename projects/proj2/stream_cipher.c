#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream_cipher.h"

struct stream_cipher *stream_cipher_create(const char *passphrase,
                                           int pass_len) {
  struct stream_cipher *cipher = malloc(sizeof(struct stream_cipher));
  cipher->passphrase = passphrase;
  cipher->input_len = strlen(passphrase) + 2 + MD5_DIGEST_LENGTH;
  cipher->output_str = malloc(cipher->input_len + 1);
  cipher->counter = 0;

  // Hash the initial passphrase
  MD5((unsigned char *)passphrase, pass_len, cipher->buffer);
  return cipher;
}

void stream_cipher_generate_key(struct stream_cipher *cipher,
                                unsigned char key_output[BITS_PER_ITERATION]) {

  sprintf((char *)&cipher->output_str[MD5_DIGEST_LENGTH], "%02d%s",
          cipher->counter, cipher->passphrase);
  memcpy(cipher->output_str, cipher->buffer, MD5_DIGEST_LENGTH);
  MD5(cipher->output_str, cipher->input_len, cipher->buffer);
  memcpy(key_output, cipher->buffer, sizeof(cipher->buffer) / 2);
  fwrite(cipher->buffer, sizeof(char), sizeof(cipher->buffer), stdout);
  if (++cipher->counter == 100) {
    cipher->counter = 0;
  }
}

int stream_cipher_get_random_bit(struct stream_cipher *cipher) {
  int bit_index = cipher->bits_read % BITS_PER_ITERATION;
  // Check if we need to generate more bits
  if (bit_index == 0) {
    stream_cipher_generate_key(cipher, cipher->random_bits);
  }

  int random_bit_index = cipher->bits_read / BITS_PER_ITERATION;
  int bit_value =
      (cipher->random_bits[random_bit_index] & (0x01 << (7 - bit_index))) != 0;
  cipher->bits_read++;
  return bit_value;
}
