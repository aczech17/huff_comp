#include "word_reader.h"
#include <stdbool.h>
#include <stdlib.h>

Word_reader* open_file(const char* filename, int word_size)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
        return NULL;
    
    Word_reader* word_reader = malloc(sizeof(Word_reader));
    
    word_reader->file = file;
    word_reader->word_size = word_size;
    word_reader->latest_byte = 0; // ?
    word_reader->in_half_byte = false;

    return word_reader;
}

static size_t load_next_byte(Word_reader* word_reader)
{
    return fread(&word_reader->latest_byte, 1, 1, word_reader->file);
}

static void push_bits(Word* word, char byte, int from, int to)
{
    int i;
    for (i = to; i >= from; i--)
        push_bit(word, byte >> i);
}

Word* get_word(Word_reader* word_reader)
{
    Word* word = new_word();

    if (word_reader->word_size == 16)
    {
        if (load_next_byte(word_reader) == 0)
            return word;

        push_bits(word, word_reader->latest_byte, 0, 7);

        if (load_next_byte(word_reader) == 0)
            return word;

        push_bits(word, word_reader->latest_byte, 0, 7);
    }
    else if (word_reader->word_size == 12)
    {
        if (word_reader->in_half_byte)
        {
            push_bits(word, word_reader->latest_byte, 0, 3);

            if (load_next_byte(word_reader) == 0)
            {
                word_reader->in_half_byte = false; // is it necessary?
                return word;
            }

            push_bits(word, word_reader->latest_byte, 0, 7);
            word_reader->in_half_byte = false;
        }
        else
        {
            if (load_next_byte(word_reader) == 0)
                return word;

            push_bits(word, word_reader->latest_byte, 0, 7);

            if (load_next_byte(word_reader) == 0)
                return word;

            push_bits(word, word_reader->latest_byte, 4, 7);
            word_reader->in_half_byte = true;
        }
    }
    else
    {
        if (load_next_byte(word_reader) == 0)
            return word;

        push_bits(word, word_reader->latest_byte, 0, 7);
    }
    

    return word;
}

void close_file(Word_reader* word_reader)
{
    fclose(word_reader->file);
    free(word_reader);
}
