#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream_cipher.h"

struct stream_cipher *stream_cipher_create(const char *passphrase) {
  struct stream_cipher *cipher = malloc(sizeof(struct stream_cipher));
  cipher->passphrase = strdup(passphrase);
  cipher->str_len = strlen(passphrase) + 2 + MD5_DIGEST_LENGTH;
  cipher->s = (char *)malloc(cipher->str_len + 1);
  cipher->counter = 0;

  // Hash the initial passphrase
  MD5((unsigned char *)passphrase, strlen(passphrase), cipher->buffer);
  return cipher;
}

void stream_cipher_generate_bytes(struct stream_cipher *cipher, int n,
                                  unsigned char *output_buffer) {
  int remaining = n;
  while (remaining > 0) {
    sprintf(&cipher->s[MD5_DIGEST_LENGTH], "%02d%s", cipher->counter,
            cipher->passphrase);
    memcpy(cipher->s, cipher->buffer, MD5_DIGEST_LENGTH);

    MD5((unsigned char *)cipher->s, cipher->str_len, cipher->buffer);

    int bytes_to_output = 0;
    if (remaining - BYTES_PER_ITERATION >= 0) {
      bytes_to_output = BYTES_PER_ITERATION;
      remaining -= BYTES_PER_ITERATION;
    } else {
      bytes_to_output = remaining;
      remaining = 0;
    }

    memcpy(output_buffer, cipher->buffer, bytes_to_output);
    output_buffer += bytes_to_output;

    if (++cipher->counter == 100) {
      cipher->counter = 0;
    }
  }
}

void stream_cipher_close(struct stream_cipher *cipher) {
  free(cipher->s);
  free((char *)cipher->passphrase);
}

int stream_cipher_get_random_bit(struct stream_cipher_bit_reader *cipher) {
  int bit_index = cipher->bits_read % BYTES_PER_ITERATION;
  // Check if we need to generate more bits
  if (bit_index == 0) {
    stream_cipher_generate_bytes(cipher->cipher, BYTES_PER_ITERATION,
                                 cipher->random_bits);
  }

  int random_bit_index = cipher->bits_read / BYTES_PER_ITERATION;
  int bit_value =
      (cipher->random_bits[random_bit_index] & (0x01 << (7 - bit_index))) != 0;
  cipher->bits_read++;
  return bit_value;
}
