#ifndef HUFF_COMP_WORD
#define HUFF_COMP_WORD

#include <stdbool.h>

typedef char Bit;

typedef struct Word
{
    char* data;
    int size;
    int capacity;
}Word;

Word* new_word();
int push_bit(Word* word, Bit bit);
int pop_bit(Word* word);
Bit get_nth_bit(const Word* word, int index);
void free_word(Word* word);
bool equals(const Word*, const Word*);
Word* copy_word(Word*);
Word* get_word_from_number(int number, int length);

#endif //HUFF_COMP_WORD
