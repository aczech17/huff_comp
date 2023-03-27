#include "compress.h"
#include "node_array.h"
#include "dictionary.h"
#include "word_reader.h"
#include "word_writer.h"
#include "word.h"

static Dictionary* get_dictionary(Word_reader* reader)
{
    Node_array* node_array = new_node_array();

    Word* word;
    while (true)
    {
        word = get_word(reader);
        if (word->size != 0)
        {
            increment_word(node_array, word);
            free_word(word);
        }
        else
        {
            free_word(word);
            break;
        }
    }

    while (node_array->size > 1)
    {
        merge_2_nodes(node_array);
        sort_array(node_array);
    }


    Tree_node* root = node_array->arr[0];
    free_node_array(node_array);

    
    Dictionary* dict = new_dictionary();
    fill_dictionary(dict, root);
    free_tree(root);

    int word_size = reader->word_size;
    int i;
    for (i = 0; i < dict->size; i++)
    {
        if (dict->words[i]->size != word_size)
        {
            // swap with the last word
            // swap words[i] with word[dict->size - 1]
            Word* tmp = dict->words[i];
            dict->words[i] = dict->words[dict->size - 1];
            dict->words[dict->size - 1] = tmp;

            // swap codewords[i] with codewords[dict->size - 1]
            tmp = dict->codewords[i];
            dict->codewords[i] = dict->codewords[dict->size - 1];
            dict->codewords[dict->size - 1] = tmp;

            break;
        }
    }

    return dict;
}

static int get_bit_count(int number)
{
    if (number == 0)
        return 1;

    int count = 0;
    while (number != 0)
    {
        count++;
        number /= 2;
    }

    return count;
}

static void write_dictionary(const Dictionary* dict, Word_writer* writer)
{
    FILE* file = writer->file;
    const char* signature = "CRSK";
    fprintf(file, "%s", signature);

    char padding = 0; // for now it's zero
    fwrite(&padding, 1, 1, file);

    int word_length = dict->words[0]->size;
    Word* word_length_coded = new_word();
    switch (word_length)
    {
        case 8:
        {
            push_bit(word_length_coded, 0);
            push_bit(word_length_coded, 0);
            break;
            
        }
        case 12:
        {
            push_bit(word_length_coded, 0);
            push_bit(word_length_coded, 1);
            break;
        }
        case 16:
        {
            push_bit(word_length_coded, 1);
            push_bit(word_length_coded, 0);
            break;
        }
        default:
            break;
    }
    write_word(writer, word_length_coded);
    free_word(word_length_coded);

    int max_codeword_length = get_max_codeword_length(dict);
    Word* max_codeword_length_coded = get_word_from_number(max_codeword_length, word_length);
    write_word(writer, max_codeword_length_coded);
    free_word(max_codeword_length_coded);

    int codeword_length_bit_count = get_bit_count(max_codeword_length);
    Word* dict_entry_count_minus_one_coded = get_word_from_number(dict->size - 1, word_length);
    write_word(writer, dict_entry_count_minus_one_coded);
    free_word(dict_entry_count_minus_one_coded);


    int i;
    for (i = 0; i < dict->size; i++)
    {
        Word* word = dict->words[i];
        Word* codeword = dict->codewords[i];

        write_word(writer, word);

        Word* codeword_size_coded = get_word_from_number(codeword->size, codeword_length_bit_count);
        write_word(writer, codeword_size_coded);
        free_word(codeword_size_coded);

        write_word(writer, codeword);
    }
}

/*
    error codes:
    0 - OK
    1 - could not open the file
    2 - could not create the new file
    555 - other
*/

int compress_file(const char* input_filename, const char* output_filename, int word_size)
{
    Word_reader* reader = open_file(input_filename, word_size);
    if (reader == NULL)
        return 1;

    Dictionary* dict = get_dictionary(reader);
    close_reader(reader);

    Word_writer* writer = create_file(output_filename);
    if (writer == NULL)
    {
        free_dictionary(dict);
        return 2;
    }

    reader = open_file(input_filename, word_size);
    if (reader == NULL)
    {
        close_writer(writer);
        return 1;
    }

    write_dictionary(dict, writer);
    Word* word;

    while (true)
    {
        word = get_word(reader);
        if (word->size != 0)
        {
            Word* codeword = get_codeword(dict, word);
            write_word(writer, codeword);
            free_word(word);
        }
        else
        {
            if (writer->bits_filled > 0) // dump the rest
            {
                fwrite(&writer->latest_byte, 1, 1, writer->file);
                char padding = 8 - (char)writer->bits_filled;
                fseek(writer->file, 4, SEEK_SET);
                fwrite(&padding, 1, 1, writer->file);
            }

            free_word(word);
            break;
        }
    }
    
    free_dictionary(dict);
    close_reader(reader);
    close_writer(writer);
    return 0;
}

