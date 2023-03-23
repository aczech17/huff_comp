#include "compress.h"
#include "node_array.h"
#include "dictionary.h"
#include "word_reader.h"
#include "word_writer.h"

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
    return dict;
}

static void print_word(const Word* word, FILE* file)
{
    int i;
    for (i = 0; i < word->size; i++)
    {
        Bit bit = get_nth_bit(word, i);
        fprintf(file, "%d", bit);
    }
    //printf("\n");
}

static void print_dictionary(const Dictionary* dict, FILE* file)
{
    //printf("%d\n", dict->size);
    int i;
    for (i = 0; i < dict->size; i++)
    {
        print_word(dict->words[i], file);
        fprintf(file, " ");
        print_word(dict->codewords[i], file);
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}

static void write_dictionary(const Dictionary* dict, Word_writer* writer)
{
    FILE* file = writer->file;
    const char* signature = "CRSK";
    fprintf(file, "%s", signature);
    
    print_dictionary(dict, file);
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
                fwrite(&writer->latest_byte, 1, 1, writer->file);

            free_word(word);
            break;
        }
    }

    free_dictionary(dict);
    close_reader(reader);
    close_writer(writer);
    return 0;
}