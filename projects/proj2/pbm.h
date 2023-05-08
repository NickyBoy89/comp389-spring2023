#include <stdio.h>

#define PBM_MAGIC_NUMBER "P4"
#define PBM_MAGIC_NUMBER_BYTES 3

// `pbm_file` represents an image in PBM binary image format
struct pbm_file {
  // `width` is the width of the PBM file in bits
  int width;
  // `height` is the height in bits
  int height;
  char **rows;
};

// `read_pbm_file` reads in an array of bytes and returns a pointer to a
// new `pbm_file` struct on the heap
//
// When freeing the memory returned from this function, call `close_pbm_file`
// first to free the internal structures
struct pbm_file *read_pbm_file(const char data[], int data_size);

// `pbm_file_display` prints out the PBM file to the terminal, displaying a
// space character (" ") for each zero bit, and a "#" for each one bit
void pbm_file_display(const struct pbm_file *file);

// `pbm_file_close` cleans up and frees all memory associated with a `pbm_file`
// struct. This should be called before freeing the `pbm_file` itself
void pbm_file_close(struct pbm_file *file);

// `pbm_width_bytes` returns the width of the PBM file, including padding
int pbm_width_bytes(const struct pbm_file *file);

// `pbm_file_write_to_file` writes the PBM data to the specified file
void pbm_file_write_to_file(struct pbm_file *file, FILE *output);
