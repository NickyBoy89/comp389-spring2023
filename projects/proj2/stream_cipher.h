#include <openssl/md5.h>

#define BYTES_PER_ITERATION MD5_DIGEST_LENGTH / 2

struct stream_cipher {
  unsigned char buffer[MD5_DIGEST_LENGTH];
  const char *passphrase;
  int str_len;
  int counter;
  char *s;
};

struct stream_cipher *stream_cipher_create(const char *passphrase);

void stream_cipher_generate_bytes(struct stream_cipher *cipher, int n,
                                  unsigned char *buffer);

void stream_cipher_close(struct stream_cipher *cipher);

struct stream_cipher_bit_reader {
  struct stream_cipher *cipher;
  unsigned char random_bits[BYTES_PER_ITERATION];
  int bits_read;
};

int stream_cipher_get_random_bit(struct stream_cipher_bit_reader *cipher);
