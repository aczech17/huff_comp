#include "word.h"
#include <stdlib.h>

static int resize(Word* word)
{
    int new_capacity = 2 * word->capacity;
    char* new_data = realloc(word->data, new_capacity);
    if (new_data == NULL)
    {
        free(word->data);
        return 1;
    }

    int i;
    for (i = word->capacity; i < new_capacity; i++)
        new_data[i] = 0;

    word->data = new_data;
    word->capacity = new_capacity;

    return 0;
}

Word* new_word()
{
    Word* word = malloc(sizeof(Word));

    if (word == NULL)
        return NULL;

    word->size = 0;
    word->capacity = 1;

    word->data = malloc(word->capacity);
    word->data[0] = 0;

    return word;
}

int push_bit(Word* word, Bit bit)
{
    if (word->size + 1 > 8 * word->capacity)
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

Bit get_nth_bit(const Word* word, int index)
{
    int byte_index = index / 8;
    int bit_index = index % 8;

    char c = word->data[byte_index] >> (7 - bit_index);
    return (Bit)(c & 1);
}

void free_word(Word* word)
{
    free(word->data);
    word->data = NULL;
    free(word);
    word = NULL; // safety first
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