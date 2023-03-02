#include "dictionary.h"
#include <stdlib.h>

/*
     Word* words;
    Word* codewords;
    int size;
    int capacity;
*/

Dictionary* new_dictionary()
{
    Dictionary* dict = malloc(sizeof(Dictionary));

    if (dict == NULL)
        return NULL;

    dict->size = 0;
    dict->capacity = 4;

    dict->words = malloc(dict->capacity * sizeof(Word));
    dict->codewords = malloc(dict->capacity * sizeof(Word));

    return dict;
}

Word* get_codeword(const Dictionary* dict, const Word* word)
{
    int i;
    for (i = 0; i < dict->size; i++)
    {
        if (equals(&dict->words[i], word))
        {
            return &dict->codewords[i];
        }
    }

    return NULL;
}

static int resize(Dictionary* dict)
{
    int new_capacity = 2 * dict->capacity;

    Word* new_words = realloc(dict->words, new_capacity * sizeof(Word));
    Word* new_codewords = realloc(dict->codewords, new_capacity * sizeof(Word));

    if (new_words == NULL || new_codewords == NULL)
        return 1;

    dict->words = new_words;
    dict->codewords = new_codewords;
    dict->capacity = new_capacity;

    return 0;
}

int push_codeword(Dictionary* dict, Word* word, Word* codeword)
{
    if (get_codeword(dict, word)) // it already exists in the dictionary
        return 1;

    if (dict->size + 1 > dict->capacity)
    {
        if (resize(dict)) // fail
            return 1;
    }

    // move
    dict->words[dict->size] = *word;
    dict->codewords[dict->size] = *codeword;

    free(word);
    free(codeword);

    word = NULL;
    codeword = NULL;

    dict->size++;
}

void free_dictionary(Dictionary* dict)
{
    int i;
    for (i = 0; i < dict->size; i++)
    {
        free_word(&dict->words[i]);
        free_word(&dict->codewords[i]);
    }

    free(dict->words);
    free(dict->codewords);
    free(dict);
}