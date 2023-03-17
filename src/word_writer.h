#ifndef HUFF_COMP_WORD_WRITER
#define HUFF_COMP_WORD_WRITER

#include <stdio.h>
#include "word.h"

typedef struct Word_writer
{
    FILE* file;
    char latest_byte;
    int bits_filled;
}Word_writer;

Word_writer* create_file(const char* filename);
void write_word(Word_writer*, Word*);
void close_writer(Word_writer*);

#endif // HUFF_COMP_WORD_WRITER
