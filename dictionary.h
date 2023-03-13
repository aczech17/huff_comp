#ifndef HUFF_COMP_DICTIONARY
#define HUFF_COMP_DICTIONARY

#include "word.h"
#include "tree_node.h"

typedef struct
{
    Word** words;
    Word** codewords;
    int size;
    int capacity;
}Dictionary;

Dictionary* new_dictionary();
Word* get_codeword(const Dictionary* dict, const Word* word);
int push_codeword(Dictionary* dict, Word* word, Word* codeword);
void fill_dictionary(Dictionary* dictionary, Tree_node* tree);
void free_dictionary(Dictionary*);

#endif // HUFF_COMP_DICTIONARY
