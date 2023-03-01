#ifndef HUFF_COMP_TREE_NODE
#define HUFF_COMP_TREE_NODE

#include "word.h"

typedef struct Tree_node
{
    Word* word;
    struct Tree_node* left;
    struct Tree_node* right;
}Tree_node;

Tree_node* new_node(Word* word, Tree_node* left, Tree_node* right);
Tree_node* merge_nodes(Tree_node* left, Tree_node* right);

#endif // HUFF_COMP_TREE_NODE
