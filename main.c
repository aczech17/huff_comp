#include <stdio.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"
#include "tree_node.h"

void print_word(const Word* word)
{
    int i;
    for (i = 0; i < word->size; i++)
    {
        Bit bit = get_nth_bit(word, i);
        printf("%d", bit);
    }
    printf("\n");
}

int main(int argc, char** argv)
{

/*
    Tree_node* tree = NULL;
    

    while (mamy kolejne słowo word)
    {
        increment_word(tree, word);
    }
*/



    if (argc < 3)
    {
        fprintf(stderr, "Zesrałem się.\n");
        return 1;
    }
    
    Dictionary* dict = new_dictionary();
    int i, j;
    for (i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        Word* word = new_word();
        Word* codeword = new_word();
        for (j = 0; arg[j]; j++)
        {
            push_bit(word, arg[j] - '0');
            push_bit(codeword, arg[j] - '0');
        }
        push_codeword(dict, word, codeword);
    }
    
    for (i = 0; i < dict->size; i++)
    {
        Word* word = &dict->words[i];
        Word* codeword = get_codeword(dict, word);

        print_word(word);
        print_word(codeword);
    }
    
    free_dictionary(dict);
    return 0;
}
