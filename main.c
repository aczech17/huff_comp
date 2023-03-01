#include <stdio.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"

void print_word(const Word* word)
{
    int i;
    for (i = 0; i < word->size; i++)
    {
        Bit bit = get_nth_bit(word, i);
        printf("%d", bit);
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Zesrałem się.\n");
        return 1;
    }

    Word* word = new_word();

    int i;
    for (i = 0; argv[1][i]; i++)
    {
        push_bit(word, argv[1][i] - '0');
    }

    Word* codeword = new_word();
    for (i = 0; argv[2][i]; i++)
    {
        push_bit(codeword, argv[2][i] - '0');
    }

    Word word_copy = *word;

    Dictionary* dict = new_dictionary();
    push_codeword(dict, word, codeword);

    Word* found_codeword = get_codeword(dict, &word_copy);

    print_word(found_codeword);

    //free_word(&word_copy);
    free_dictionary(dict);
    return 0;
}
