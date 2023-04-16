#include <stdio.h>

// `base64_encode` takes in an array of bytes and returns a pointer to its
// base64 string representation
void base64_encode(FILE *fd);

// `base64_decode` takes in a file encoded in base64 and returns its original
// string representation
void base64_decode(FILE *fd);

// `char_to_base64_ascii` takes in an ascii character corresponding to the
// base64 ascii table, and returns the value in the base64 ascii table
unsigned char char_to_base64_ascii(unsigned char input);

char *array_to_bitstring(char data[], int size);
