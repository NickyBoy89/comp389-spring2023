// `base64_encode` takes in an array of bytes and returns a pointer to its base64
// string representation
//
// Callers should free the string returned by this function after use
char *base64_encode(const char *data, int data_size);
