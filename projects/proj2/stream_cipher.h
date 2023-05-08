#include <openssl/md5.h>

#define BITS_PER_ITERATION 8

struct stream_cipher {
  unsigned char buffer[MD5_DIGEST_LENGTH];
  const char *passphrase;
  int input_len;
  int counter;
  unsigned char *output_str;
  unsigned char random_bits[BITS_PER_ITERATION];
  int bits_read;
};

struct stream_cipher *stream_cipher_create(const char *passphrase,
                                           int pass_len);

void stream_cipher_generate_key(struct stream_cipher *cipher,
                                unsigned char key_output[BITS_PER_ITERATION]);

int stream_cipher_get_random_bit(struct stream_cipher *cipher);
