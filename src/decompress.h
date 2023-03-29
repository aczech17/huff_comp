#ifndef HUFF_COMP_DECOMPRESS
#define HUFF_COMP_DECOMPRESS

#include <stdio.h>

typedef struct 
{
    FILE* file;
    int bit_number;
}Decompress_reader;

int decompress_file(const char* input_filename, const char* output_filename);

#endif // HUFF_COMP_DECOMPRESS
