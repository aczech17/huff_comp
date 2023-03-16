#ifndef HUFF_COMP_WORD_READER
#define HUFF_COMP_WORD_READER

#include <stdio.h>
#include <stdbool.h>
#include "word.h"

typedef struct Word_reader
{
    FILE* file;
    int word_size;
    char latest_byte;
    bool in_half_byte;
}Word_reader;

Word_reader* open_file(const char* filename, int word_size);
Word* get_word(Word_reader* word_reader);
void close_file(Word_reader* word_reader);

#endif // HUFF_COMP_WORD_READER
