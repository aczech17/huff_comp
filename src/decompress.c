#include "decompress.h"
//#include "word_reader.h"
#include "word.h"
#include <stdio.h>
#include "compress.h"
#include "dictionary.h"

static Bit get_bit_from_file(FILE* file, int n)
{
    int byte_number = n / 8;
    int bit_number = n % 8;
    fseek(file, byte_number, SEEK_SET);

    char byte = (char)fgetc(file);
    
    return byte >> (7 - bit_number);
}

static Word* get_word_from_file(FILE* file, int bit_number, int length)
{
    Word* word = new_word();
    int i;
    for (i = bit_number; i < bit_number + length; i++)
    {
        Bit bit = get_bit_from_file(file, i);
        push_bit(word, bit);
    }

    return word;
}

int decompress_file(const char* input_filename, const char* output_filename)
{
    FILE* input = fopen(input_filename, "rb");
    if (input == NULL)
        return 1;

    Dictionary* dict = new_dictionary();
    
    fseek(input, 6, SEEK_SET);
    int last_word_size = fgetc(input);
    
    fseek(input, 7, SEEK_SET);
    char byte = (char)fgetc(input);

    int word_size;
    switch (byte >> 6)
    {
        case 0:
            word_size = 8;
            break;
        case 1:
            word_size = 12;
            break;
        case 2:
            word_size = 16;
            break;
        default:
            {
                fclose(input);
                return 2;
            }
    }

    int bit_number = 7 * 8 + 2;
    Word* max_codeword_length_coded = get_word_from_file(input, bit_number, word_size);
    free_word(max_codeword_length_coded);
    bit_number += word_size;

    int max_codeword_length = get_number_from_word(max_codeword_length_coded);
    int codeword_bit_count = get_bit_count(max_codeword_length);

    Word* pair_count_coded = get_word_from_file(input, bit_number, word_size);
    bit_number += word_size;
    free_word(pair_count_coded);

    int pair_count = get_number_from_word(pair_count_coded) + 1;

    int i;
    for (i = 0; i < pair_count; i++)
    {
        Word* word;
        if (i < pair_count - 1)
        {
            word = get_word_from_file(input, bit_number, word_size);
            bit_number += word_size;
        }
        else
        {
            word = get_word_from_file(input, bit_number, last_word_size);
            bit_number += last_word_size;
        }

        Word* codeword_length_coded = get_word_from_file(input, bit_number, codeword_bit_count);
        bit_number += codeword_bit_count;

        int codeword_length = get_number_from_word(codeword_length_coded);
        free_word(codeword_length_coded);

        Word* codeword = get_word_from_file(input, bit_number, codeword_length);
        bit_number += codeword_length;

        push_codeword(dict, word, codeword);
    }

}
