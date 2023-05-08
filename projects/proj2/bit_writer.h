struct bit_writer {
  unsigned char *data;
  int capacity;
  int written;
};

struct bit_writer *init_bit_writer(int capacity);

void bit_writer_print_data(const struct bit_writer *writer);

void bit_writer_write_bit(struct bit_writer *writer, int bit_value);

void bit_writer_write_bytes_unaligned(struct bit_writer *writer,
                                      unsigned char *bytes, int num_written);

void bit_writer_reset(struct bit_writer *writer);

void bit_writer_close(struct bit_writer *writer);
