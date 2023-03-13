#include "node_list.h"
#include "tree_node.h"
#include <stddef.h>

void increment_word(Tree_node** list, Word* word)
{
    if (*list == NULL) // the list is empty yet
    {
        Tree_node* node = new_node((Word*)word, 1);
        *list = node;
        return;
    }

    Tree_node* i;
    for (i = *list; i->right; i = i->right)
    {
        if (equals(i->word, word))
        {
            i->frequency++;
            return;
        }
    }

    if (equals(i->word, word))
    {
        i->frequency++;
        return;
    }

    i->right = new_node(word, 1);
}
