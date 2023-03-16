#include "compress.h"
#include "node_array.h"

Dictionary* get_dictionary(Word_reader* reader)
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
