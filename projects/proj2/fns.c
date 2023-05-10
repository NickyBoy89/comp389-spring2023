/*
 * Project 2 of COMP389: Cryptography
 * Function: stream, encrypt, merge, and decrypt
 * File Name: fns.c
 */

#include <math.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "stream_cipher.h"

#define BUFFER_LEN 8

/*
 * a simple stream cipher utilizing MD5 Message-Digest Algorithm
 * Parameters:
 * p: pointer to locate the pass phrase
 * l: number of bytes to output
 * fp: file pointer pointing to the specified temporary file or stdout
 *
 * Return value:
 * on success, return 0; else return -1
 */
int stream(char *p, int l, FILE *fp) {
  struct stream_cipher *cipher = stream_cipher_create(p);

  unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * l);

  stream_cipher_generate_bytes(cipher, l, buffer);
  int written = fwrite(buffer, sizeof(unsigned char), l, fp);
  if (ferror(fp)) {
    perror("Error writing stream to output file");
    return -1;
  }
  if (written != l) {
    printf("Error writing stream to file: wrote %d, expected %d\n", written, l);
    return -1;
  }

  free(buffer);
  stream_cipher_close(cipher);
  free(cipher);

  return 0;
}

/*
 * Encrypts an input PBM file with the simple stream cipher based on 4x data
 * expension visual cryptography by Naor and Shamir Parameters: p: pointer to
 * locate the pass phrase out: pointer to locate the output filename string fp:
 * file pointer pointing to the specified input file or stdin
 *
 * Return value:
 * on success, return 0; else return -1
 */
int encrypt(char *p, char *out, FILE *fp) {
  return 0; // FIXME
}

/*
 * Merge two input encrypted PBM files
 * Parameters:
 * fp1: file pointer pointing to the specified input file #1
 * fp2: file pointer pointing to the specified input file #2
 *
 * Return value:
 * on success, return 0; else return -1
 */
int merge(FILE *fp1, FILE *fp2) {
  return 0; // FIXME
}

/*
 * Decrypt either stdin or a specified merged PBM input file
 * Parameters:
 * fp: file pointer pointing to either the specified input file or stdin
 *
 * Return value:
 * on success, return 0; else return -1
 */
int decrypt(FILE *fp) {
  return 0; // FIXME
}
