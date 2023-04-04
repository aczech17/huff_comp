#include "word.h"
#include <stdlib.h>

/*
    char* data;
    int size;
    int capacity;
*/

static int resize(Word* word)
{
    int new_bytes_allocated = 2 * word->bytes_allocated;
    char* new_data = realloc(word->data, new_bytes_allocated);
    if (new_data == NULL)
    {
        free(word->data);
        return 1;
    }
    word->data = new_data;

    int byte_index;
    for (byte_index = word->bytes_allocated; byte_index < new_bytes_allocated; byte_index++)
        word->data[byte_index] = 0;

    word->bytes_allocated = new_bytes_allocated;

    return 0;
}

Word* new_word()
{
    Word* word = malloc(sizeof(Word));

    if (word == NULL)
        return NULL;

    word->size = 0;
    word->bytes_allocated = 2;

    word->data = malloc(word->bytes_allocated);
    int i;
    for (i = 0; i < word->bytes_allocated; i++)
        word->data[i] = 0;

    return word;
}

Word* get_word_from_number(int number, int length)
{   
    Word* word = new_word();
    int i;
    for (i = length - 1; i >= 0; i--)
    {
        Bit bit = (number >> i) % 2 == 0 ? 0 : 1;
        push_bit(word, bit);
    }

    return word;
}

int push_bit(Word* word, Bit bit)
{
    if (word->size + 1 > 8 * word->bytes_allocated)
    {
        // resize (optionally fail)
        if (resize(word) == 1)
            return 1; // fial
    }

    int byte_index = word->size / 8;
    int bit_index = word->size % 8;

    word->data[byte_index] |= (bit << (7 - bit_index));
    word->size++;

    return 0;
}

void pop_bit(Word* word)
{
    if (word->size <= 0)
        return;

    int byte_index = (word->size - 1) / 8;
    int bit_index = (word->size - 1) % 8;

    word->data[byte_index] &= (~(1 << (7 - bit_index)));

    word->size--;
}

Bit get_nth_bit(const Word* word, int index)
{
    int byte_index = index / 8;
    int bit_index = index % 8;

    char c = word->data[byte_index] >> (7 - bit_index);
    return (Bit)(c & 1);
}

void free_word(Word* word)
{
    if (word == NULL)
        return;

    free(word->data);
    free(word);
}

bool equals(const Word* w1, const Word* w2)
{
    if (w1->size != w2->size)
        return false;

    int i;
    for (i = 0; i < w1->size; i++)
    {
        Bit b1 = get_nth_bit(w1, i);
        Bit b2 = get_nth_bit(w2, i);

        if (b1 != b2)
            return false;
    }

    return true;
}

Word* copy_word(Word* word)
{
    Word* copy = new_word();


    int i;
    for (i = 0; i < word->size; i++)
        push_bit(copy, get_nth_bit(word, i));

    return copy;
}

int get_number_from_word(const Word* word)
{
    int number = 0;
    int i;

    for (i = 0; i < word->size; i++)
    {
        Bit bit = get_nth_bit(word, i);
        number = number << 1;
        number = number | bit;
    }

    return number;
}
