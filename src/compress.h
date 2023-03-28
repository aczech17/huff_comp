#ifndef HUFF_COMP_COMPRESS
#define HUFF_COMP_COMPRESS


/*
    error codes:
    0 - OK
    1 - could not open the file
    2 - could not create the new file
    555 - other
*/
int compress_file(const char* input_filename, const char* output_filename, int word_size);
int get_bit_count(int number);

#endif // HUFF_COMP_COMPRESS
