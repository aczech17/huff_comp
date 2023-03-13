#include "tree_node.h"

typedef struct Node_array
{
    Tree_node** arr;
    int size;
    int capacity;
}Node_array;


Node_array* new_node_array();
void increment_word(Node_array* node_array, Word* word);
void sort_array(Node_array* node_array);
