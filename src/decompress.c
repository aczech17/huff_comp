#include "decompress.h"
#include "word_writer.h"
#include "word.h"
#include <stdio.h>
#include "compress.h"
#include "dictionary.h"
#include <stdlib.h>
#include <stdbool.h>

static Bit get_bit_from_file(FILE* file, int n)
{
    int byte_number = n / 8;
    int bit_number = n % 8;
    fseek(file, byte_number, SEEK_SET);

    int byte = fgetc(file);
    
    Bit bit = (byte >> (7 - bit_number)) % 2;
    return bit;
}

static Decompress_reader* new_decompress_reader(const char* input_filename)
{
    Decompress_reader* reader = malloc(sizeof(Decompress_reader));
    reader->file = fopen(input_filename, "rb");
    reader->bit_number = 0;

    return reader;
}

static Word* get_word_from_file(Decompress_reader* reader, int length)
{
    Word* word = new_word();
    int i;
    for (i = reader->bit_number; i < reader->bit_number + length; i++)
    {
        Bit bit = get_bit_from_file(reader->file, i);
        push_bit(word, bit);
    }
    reader->bit_number += length;

    return word;
}

int decompress_file(const char* input_filename, const char* output_filename)
{
    FILE* input = fopen(input_filename, "rb");
    if (input == NULL)
        return 1;

    fseek(input, 0, SEEK_END);
    int input_length = ftell(input) * 8;

    fseek(input, 5, SEEK_SET);
    int padding = fgetc(input);

    Dictionary* dict = new_dictionary();
    
    fseek(input, 6, SEEK_SET);
    int last_word_size = fgetc(input);
    
    fseek(input, 7, SEEK_SET);
    int byte = fgetc(input);

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
    fclose(input);

    Decompress_reader* reader = new_decompress_reader(input_filename);


    reader->bit_number = 7 * 8 + 2;
    Word* max_codeword_length_coded = get_word_from_file(reader, word_size);

    int max_codeword_length = get_number_from_word(max_codeword_length_coded);
    free_word(max_codeword_length_coded);

    int codeword_bit_count = get_bit_count(max_codeword_length);
    Word* pair_count_coded = get_word_from_file(reader, word_size);

    int pair_count = get_number_from_word(pair_count_coded) + 1;
    free_word(pair_count_coded);


    int i;
    for (i = 0; i < pair_count; i++)
    {
        Word* word;
        if (i < pair_count - 1)
        {
            word = get_word_from_file(reader, word_size);
        }
        else
        {
            word = get_word_from_file(reader, last_word_size);
        }

        Word* codeword_length_coded = get_word_from_file(reader, codeword_bit_count);

        int codeword_length = get_number_from_word(codeword_length_coded);
        free_word(codeword_length_coded);

        Word* codeword = get_word_from_file(reader, codeword_length);

        push_codeword(dict, word, codeword);
    }

    Word_writer* writer = create_file(output_filename);
    while (reader->bit_number < input_length - padding)
    {
        //printf("%d %d\n", reader->bit_number, input_length - padding);
        for (i = 0; i < dict->size; i++)
        {
            Word* word_from_file = get_word_from_file(reader, dict->codewords[i]->size);
            reader->bit_number -= dict->codewords[i]->size;

            if (equals(word_from_file, dict->codewords[i]))
            {
                //printf("%d ", reader->bit_number);
                write_word(writer, dict->words[i]);
                reader->bit_number += dict->codewords[i]->size;
                //printf("%d ", reader->bit_number);
                break;
            }
            //puts("nie znaleźliśmy codeworda");
        }
        //printf("%d\n", reader->bit_number);
    }

    if (writer->bits_filled > 0) // dump the rest
    {
        fwrite(&writer->latest_byte, 1, 1, writer->file);
    }

    close_writer(writer);
    free_dictionary(dict);

    return 0;
}
