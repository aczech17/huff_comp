#include "word_writer.h"
#include <stdlib.h>

/*
    FILE* file;
    char latest_byte;
    int bits_written;
*/

Word_writer* create_file(const char* filename)
{
    Word_writer* writer = malloc(sizeof(Word_writer));
    if (writer == NULL)
        return NULL;

    writer->file = fopen(filename, "wb");
    if (writer->file == NULL)
    {
        free(writer);
        return NULL;
    }

    writer->latest_byte = 0;
    writer->bits_filled = 0;

    return writer;
}

void write_word(Word_writer* writer, Word* word)
{
    int i = 0;
    int bits_written = 0;
    while (bits_written < word->size)
    {
        int bits_to_write = word->size - bits_written;
        int bits_to_fill = 8 - writer->bits_filled;

        int bits_to_append = bits_to_write < bits_to_fill ? bits_to_write : bits_to_fill;
        int b;
        for (b = 0; b < bits_to_append; b++)
        {
            Bit bit = get_nth_bit(word, i);
            writer->latest_byte |= (bit << (7 - writer->bits_filled));
            writer->bits_filled++;
            i++;
            bits_written++;
        }

        if (writer->bits_filled == 8)
        {
            fwrite(&writer->latest_byte, 1, 1, writer->file);
            writer->latest_byte = 0;
            writer->bits_filled = 0;
        }
    }
}

void close_writer(Word_writer* writer)
{
    fclose(writer->file);
    free(writer);
}
