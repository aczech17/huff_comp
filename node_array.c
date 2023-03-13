#include "node_array.h"
#include <stddef.h>
#include <stdlib.h>

/*
    Tree_node** arr;
    int size;
    int capacity;
*/

Node_array* new_node_array()
{
    int capacity = 10;
    Tree_node** arr = malloc(capacity * sizeof(Tree_node*));

    Node_array* node_array = malloc(sizeof(Node_array));
    node_array->arr = arr;
    node_array->size = 0;
    node_array->capacity = capacity;

    return node_array;
}

static void resize(Node_array* node_array)
{
    int new_capacity = node_array->capacity * 2;
    node_array->arr = realloc(node_array->arr, new_capacity);
    node_array->capacity = new_capacity;
}

static void add_word(Node_array* node_array, Word* word)
{
    if (node_array->size == node_array->capacity)
        resize(node_array);
    Tree_node* node = new_node(word, 1);
    node_array->arr[node_array->size++] = node;
}

void increment_word(Node_array* node_array, Word* word)
{
    int i;
    for (i = 0; i < node_array->size; i++)
    {
        if (equals(node_array->arr[i]->word, word))
        {
            node_array->arr[i]->frequency++;
            return;
        }
    }
    add_word(node_array, word);
}
