#include "tree_node.h"
#include <stdlib.h>

/*
    Word* word;
    int frequency;
    struct Tree_node* left;
    struct Tree_node* right;
*/

Tree_node* new_node(Word* word, int frequency)
{
    Tree_node* node = malloc(sizeof(Tree_node));
    node->word = word;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Tree_node* merge_nodes(Tree_node* left, Tree_node* right)
{
    Tree_node* node = new_node(NULL, left->frequency + right->frequency);
    node->left = left;
    node->right = right;

    return node;
}
