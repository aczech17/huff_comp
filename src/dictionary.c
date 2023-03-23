#include "dictionary.h"
#include <stdlib.h>

/*
     Word** words;
    Word** codewords;
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

    dict->words = malloc(dict->capacity * sizeof(Word*));
    dict->codewords = malloc(dict->capacity * sizeof(Word*));

    return dict;
}

Word* get_codeword(const Dictionary* dict, const Word* word)
{
    int i;
    for (i = 0; i < dict->size; i++)
    {
        if (equals(dict->words[i], word))
        {
            return dict->codewords[i];
        }
    }

    return NULL;
}

static int resize(Dictionary* dict)
{
    int new_capacity = 2 * dict->capacity;

    Word** new_words = realloc(dict->words, new_capacity * sizeof(Word*));
    Word** new_codewords = realloc(dict->codewords, new_capacity * sizeof(Word*));

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
    dict->words[dict->size] = word;
    dict->codewords[dict->size] = codeword;

    dict->size++;
}

static void traverse_tree(Dictionary* dict, Tree_node* tree, Word* current_word)
{
    if (tree->left)
    {
        push_bit(current_word, 0);
        traverse_tree(dict, tree->left, current_word);


        pop_bit(current_word);
        push_bit(current_word, 1);

        traverse_tree(dict, tree->right, current_word);
        pop_bit(current_word);
    }
    else // leaf
    {
        Word* word = tree->word;
        Word* codeword = copy_word(current_word);
        push_codeword(dict, word, codeword);
    }
    
}

void fill_dictionary(Dictionary* dictionary, Tree_node* tree)
{
    Word* current_word = new_word();
    if (tree->left == NULL) // only one word
    {
        push_bit(current_word, 0);
        Word* zero_word = copy_word(current_word);
        push_codeword(dictionary, tree->word, zero_word);
    }
    else
        traverse_tree(dictionary, tree, current_word);
    free_word(current_word);
}

int get_max_codeword_length(const Dictionary* dict)
{
    int max_length = -1;
    int i;
    for (i = 0; i < dict->size; i++)
    {
        if (dict->codewords[i]->size > max_length)
        {
            max_length = dict->codewords[i]->size;
        }
    }

    return max_length;
}

void free_dictionary(Dictionary* dict)
{
    int i;
    for (i = 0; i < dict->size; i++)
    {
        free_word(dict->words[i]);
        free_word(dict->codewords[i]);
    }

    free(dict->words);
    free(dict->codewords);
    free(dict);
}
