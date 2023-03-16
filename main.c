#include <stdio.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"
#include "tree_node.h"
#include "node_array.h"
#include <stdlib.h>
#include <stdbool.h>
#include "word_reader.h"

void print_word(const Word* word)
{
    int i;
    for (i = 0; i < word->size; i++)
    {
        Bit bit = get_nth_bit(word, i);
        printf("%d", bit);
    }
    //printf("\n");
}

void print_dictionary(const Dictionary* dict)
{
    //printf("%d\n", dict->size);
    int i;
    for (i = 0; i < dict->size; i++)
    {
        print_word(dict->words[i]);
        printf(" ");
        print_word(dict->codewords[i]);
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    Word_reader* reader = open_file(argv[1], 12);
    Node_array* node_array = new_node_array();

    Word* word;
    while (true)
    {
        word = get_word(reader);
        if (word->size != 0)
            increment_word(node_array, word);
        else
        {
            free_word(word);
            break;
        }

        free_word(word);
    }
    
    while (node_array->size > 1)
    {
        merge_2_nodes(node_array);
        sort_array(node_array);
    }


    Tree_node* root = node_array->arr[0];
    free_node_array(node_array);

    
    Dictionary* dict = new_dictionary();
    fill_dictionary(dict, root);

    print_dictionary(dict);

    free_tree(root);
    free_dictionary(dict);



    close_file(reader);
    return 0;
}
