#include <stdio.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"
#include <stdlib.h>
#include "word_reader.h"
#include "compress.h"

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

void print_dictionary(const Dictionary* dict)
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
 ./comp test12 -O1
01000011 0
01000001 10
01000010 11
*/


int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [compress rate (-O1 | -O2 | -O3)]";
    if (argc < 3)
    {
        fprintf(stderr, "%s\n", usage);
        return 1;
    }

    int word_size;
    if (strcmp(argv[2], "-O1") == 0)
        word_size = 8;
    else if (strcmp(argv[2], "-O2") == 0)
        word_size = 12;
    else if (strcmp(argv[2], "-O3") == 0)
        word_size = 16;
    else
    {
        fprintf(stderr, "Bad argument\n");
        return 2;
    }

    Word_reader* reader = open_file(argv[1], word_size);
    if (reader == NULL)
    {
        fprintf(stderr, "Nie udało się otworzyć pliku.\n");
        return 3;
    }

    Dictionary* dict = get_dictionary(reader);
    close_file(reader);



    Word* A = new_word(); // 01000001
    push_bit(A, 0);
    push_bit(A, 1);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 0);
    push_bit(A, 1);

    Word* c = get_codeword(dict, A);

    print_word(c);

    free_word(A);
    
    free_dictionary(dict);
    return 0;
}
