// `pbm_file` represents an image in PBM binary image format
struct pbm_file {
  int width;
  int height;
  char** rows;
  int data_size;
};

// `read_pbm_file` reads in an array of bytes and returns a pointer to a 
// new `pbm_file` struct on the stack
//
// When freeing the memory returned from this function, call `close_pbm_file` 
// first to free the internal structures
struct pbm_file* read_pbm_file(const char data[], int data_size);

// `display_pbm_file` prints out the PBM file to the terminal, displaying a
// space character (" ") for each zero bit, and a "#" for each one bit
void display_pbm_file(const struct pbm_file* file);

// `close_pbm_file` cleans up and frees all memory associated with a `pbm_file`
// struct. This should be called before freeing the `pbm_file` itself
void close_pbm_file(struct pbm_file* file);
