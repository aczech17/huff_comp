#include "compress.h"
#include "node_array.h"
#include "dictionary.h"

static Dictionary* get_dictionary(Word_reader* reader)
{
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
    free_tree(root);
    return dict;
}

static void print_word(const Word* word)
{
    int i;
    for (i = 0; i < word->size; i++)
    {
        Bit bit = get_nth_bit(word, i);
        printf("%d", bit);
    }
    //printf("\n");
}

static void print_dictionary(const Dictionary* dict)
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

/*
    error codes:
    0 - OK
    1 - could not open the file
    2 - could not create the new file
    555 - other
*/

int compress_file(const char* input_filename, const char* output_filename, int word_size)
{
    Word_reader* reader = open_file(input_filename, word_size);
    if (reader == NULL)
        return 1;

    Dictionary* dict = get_dictionary(reader);
    close_file(reader);

    print_dictionary(dict);

    free_dictionary(dict);
    return 0;
}
