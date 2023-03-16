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
    Tree_node* node = new_node(copy_word(word), 1); // node array is an owner of the new word copy
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

void sort_array(Node_array* node_array)
{
    Tree_node* temp = NULL;
    for(int i = 0; i< node_array->size - 1; i++)
        for(int j = 0;j< node_array->size - i - 1;j++)
            if(node_array->arr[j]->frequency > node_array->arr[j + 1]->frequency)
            {
                temp = node_array->arr[j];
                node_array ->arr[j] = node_array ->arr[j+1];
                node_array->arr[j+1] = temp;

            }      
}

void merge_2_nodes(Node_array* node_array)
{
    Tree_node* left = node_array->arr[0];
    Tree_node* right = node_array->arr[1];

    Tree_node* parent = merge_nodes(left, right);
    node_array->arr[0] = parent;
    node_array->arr[1] = node_array->arr[node_array->size - 1];

    node_array->size--;
}

void free_node_array(Node_array* node_array)
{
    free(node_array->arr);
    free(node_array);
}
