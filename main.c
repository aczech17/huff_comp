#include <stdio.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"
#include "tree_node.h"
#include "node_array.h"

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

void traverse(Tree_node* root)
{
    if (root->left)
        traverse(root->left);
    if (root->right)
        traverse(root->right);

    if (root->word)
        print_word(root->word);
    printf(" %d\n", root->frequency);
}

void print_dictionary(const Dictionary* dict)
{
    //printf("%d\n", dict->size);
    int i;
    for (i = 0; i < dict->size; i++)
    {
        print_word(&dict->words[i]);
        printf(" ");
        print_word(&dict->codewords[i]);
        printf("\n");
    }
}

int main(int argc, char** argv)
{

/*
    Tree_node* tree = NULL;
    

    while (mamy kolejne słowo word)
    {
        increment_word(&tree, word);
    }
*/

    /*
    A - 65 01000001
    B - 66 01000010
    C - 67 01000011
    D - 68 01000100 
    */

    Word* A = new_word();
    push_bit(A, 0);
    push_bit(A, 1);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 1);

    Word* B = new_word();
    push_bit(B, 0);
    push_bit(B, 1);
    push_bit(B, 0);
    push_bit(B, 0);
    push_bit(B, 0);
    push_bit(B, 0);
    push_bit(B, 1);
    push_bit(B, 0);

    Word* C = new_word();
    push_bit(C, 0);
    push_bit(C, 1);
    push_bit(C, 0);
    push_bit(C, 0);
    push_bit(C, 0);
    push_bit(C, 0);
    push_bit(C, 1);
    push_bit(C, 1);

    Word* D = new_word();
    push_bit(D, 0);
    push_bit(D, 1);
    push_bit(D, 0);
    push_bit(D, 0);
    push_bit(D, 0);
    push_bit(D, 1);
    push_bit(D, 0);
    push_bit(D, 0);



    Node_array* node_array = new_node_array();

    increment_word(node_array, A);

    increment_word(node_array, B);
    increment_word(node_array, B);

    increment_word(node_array, C);
    increment_word(node_array, C);
    increment_word(node_array, C);

    increment_word(node_array, D);
    increment_word(node_array, D);
    increment_word(node_array, D);
    increment_word(node_array, D);

    

    while (node_array->size > 1)
    {
        merge_2_nodes(node_array);
        sort_array(node_array);
    }


    Tree_node* root = node_array->arr[0];
    
    Dictionary* dict = new_dictionary();
    fill_dictionary(dict, root);

    print_dictionary(dict);

    /*
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
    */

    return 0;
}
