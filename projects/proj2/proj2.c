/*
 * Project 2 of COMP389: Cryptography
 * Function: commandline parsing and file reading
 * File Name: proj2.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encrypt.h"
#include "fns.h"

void usage() {
  printf("The commandline syntax for proj 2 is as follows:\n");
  printf("    proj stream -p=pphrase -l=len\n");
  printf("    proj encrypt -p=pphrase -out=name [pbmfile]\n");
  printf("    proj merge pbmfile1 pbmfile2\n");
  printf("    proj decrypt [pbmfile]\n");
  exit(1);
}

struct cli_args {
  char *passphrase;
  char *outfile;
  int len;
};

enum command_type {
  STREAM,
  ENCRYPT,
  MERGE,
  DECRYPT,
};

#define STREAM_CMD "stream"
#define ENCRYPT_CMD "encrypt"
#define MERGE_CMD "merge"
#define DECRYPT_CMD "decrypt"

int equals_index(char *input_str, int len) {
  for (int i = 0; i < len; i++) {
    if (input_str[i] == '=') {
      return i;
    }
  }
  return -1;
}

struct cli_args parse_cli_args(char *args[], int num_args) {
  struct cli_args parsed_args;
  for (int arg_index = 0; arg_index < num_args; arg_index++) {
    // Detect cli args by the leading dash
    if (args[arg_index][0] == '-') {
      int split_index = equals_index(args[arg_index], strlen(args[arg_index]));
      char *comparison = args[arg_index] + (split_index + 1);
      if (args[arg_index][1] == 'o' && args[arg_index][2] == 'u' &&
          args[arg_index][3] == 't') {
        parsed_args.outfile = strdup(comparison);
      } else {
        switch (args[arg_index][1]) {
        case 'p': {
          // Some pointer arithmetic to find the string associated with the
          // substring
          parsed_args.passphrase = strdup(comparison);
          break;
        }
        case 'l': {
          parsed_args.len = atoi(comparison);
          printf("Converting to number: %d, error: %d\n", parsed_args.len,
                 errno);
          break;
        }
        }
      }
    }
  }

  return parsed_args;
}

int main(int argc, char *argv[]) {
  enum command_type cmd;

  if (argc > 1) {
    if (strcmp(STREAM_CMD, argv[1]) == 0) {
      cmd = STREAM;
    } else if (strcmp(ENCRYPT_CMD, argv[1]) == 0) {
      cmd = ENCRYPT;
    } else if (strcmp(MERGE_CMD, argv[1]) == 0) {
      cmd = MERGE;
    } else if (strcmp(DECRYPT_CMD, argv[1]) == 0) {
      cmd = DECRYPT;
    } else {
      usage();
      return EXIT_FAILURE;
    }
  } else {
    usage();
    return EXIT_FAILURE;
  }

  if (argc > 3) {
    struct cli_args args = parse_cli_args(argv, argc);
    printf("P: %s, L: %d, Out: %s\n", args.passphrase, args.len, args.outfile);
  }

  switch (cmd) {
  case ENCRYPT:
    if (argc < 5) {
      usage();
      return EXIT_FAILURE;
    }

    printf("Opening file %s as input\n", argv[4]);
    FILE *fd = fopen(argv[4], "rb");
    if (fd == NULL) {
      perror("Error opening pbm file");
      exit(EXIT_FAILURE);
    }

    fseek(fd, 0, SEEK_END);
    int file_size = ftell(fd);
    rewind(fd);

    unsigned char *data = malloc(sizeof(unsigned char) * file_size);
    int written = fread(data, sizeof(unsigned char), file_size, fd);
    if (written != file_size) {
      printf("ERROR: Reading pbm file was incomplete. Expected %d bytes, but "
             "read %d bytes",
             file_size, written);
    }
    fclose(fd);

    struct pbm_file *infile =
        read_pbm_file((char *)data, file_size * sizeof(unsigned char));

    char *test_pass = "yesnomaybe";
    int pass_len = strlen(test_pass);

    struct stream_cipher *cipher = stream_cipher_create(test_pass, pass_len);

    // display_pbm_file(infile);

    encrypt_pbm_file(infile, cipher);
  }

  return EXIT_SUCCESS;
}
