#ifndef HUFF_COMP_ENCRYPT
#define HUFF_COMP_ENCRYPT

#include <stdbool.h>

void encrypt_file(const char* input_filename, const char* output_filename, const char* encrypt_word);
bool decrypt_file(const char* input_filename, const char* output_filename, const char* encrypt_word);
long get_file_size(const char* filename);
bool file_is_encrypted(const char* filename);

#endif // HUFF_COMP_ENCRYPT
