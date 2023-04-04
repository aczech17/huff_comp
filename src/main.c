#include <stdio.h>
#include <string.h>
#include "compress.h"
#include <ctype.h>

#include "word.h"
#include "decompress.h"

#include <stdbool.h>

typedef enum Action
{
    compress, decompress
}Action;

typedef enum EncryptionAction
{
    encrypt, decrypt
}EncryptionAction;

typedef struct 
{
    char* input_filename;
    char* output_filename;
    Action action;
    int word_size;
    bool verbose;
    char* password;
}Config;

char get_xor(const char* word)
{
    char key = 0;
    int i;
    for (i = 0; isalnum(word[i]); i++)
    {
        key = key ^ word[i];
    }

    return key;
}

void xor_file(const char* input_filename, const char* output_filename, const char* encrypt_word, EncryptionAction action)
{
    char key = get_xor(encrypt_word);
    FILE* input = fopen(input_filename, "rb");
    FILE* output = fopen(output_filename, "wb");
    
    char encrypted_signature;
    if (action == encrypt)
        encrypted_signature = 0xFF;
    else
        encrypted_signature = 0x00;
    fwrite(&encrypted_signature, 1, 1, output);
    

    char byte;
    fseek(input, 1, SEEK_SET);
    while (fread(&byte, 1, 1, input))
    {
        byte = byte^key;
        fwrite(&byte, 1, 1, output);
    }

    fclose(output);
    fclose(input);
}



long get_file_size(const char* filename)
{
    FILE* in = fopen(filename, "rb");
    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    fclose(in);
    return size;
}

bool file_is_encrypted(const char* filename)
{
    bool encrypted;
    FILE* input = fopen(filename, "rb");
    int first_byte = fgetc(input);
    if (first_byte == 0xFF)
        encrypted = true;
    else
        encrypted = false;
    fclose(input);
    return encrypted;
}

int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [output filename] (-d | [compress rate (-O1 | -O2 | -O3)]) -v? encrypt_word?";
    /*                   0    1                2                 3                                         4  (4 | 5)   */
    
    if (argc < 5)
    {
        fprintf(stderr, "%s\n", usage);
        return 1944;
    }

    const char* tmpfile_name = "temp.xor";

    Config config;

    config.input_filename = argv[1];
    config.output_filename = argv[2];
    if (strcmp(config.output_filename, tmpfile_name) == 0)
    {
        fprintf(stderr, "Zarezerwowana nazwa pliku.\n");
        return 410;
    }

    if (strcmp(argv[3], "-d") == 0)
        config.action = decompress;
    else
        config.action = compress;

    if (config.action == compress)
    {
        if (strcmp(argv[3], "-O1") == 0)
            config.word_size = 8;
        else if (strcmp(argv[3], "-O2") == 0)
            config.word_size = 12;
        else if (strcmp(argv[3], "-O3") == 0)
            config.word_size = 16;
    }

    if (strcmp(argv[4], "-v") == 0)
        config.verbose = true;
    else
        config.verbose = false;

    if (config.verbose)
    {
        if (argc >= 6)
            config.password = argv[5];
        else
            config.password = NULL;
    }
    else
    {
        if (argc >= 5)
            config.password = argv[4];
        else
            config.password = NULL;
    }

    if (get_file_size(config.input_filename) == 0)
    {
        fprintf(stderr, "The file is empty.\n");
        return 255;
    }

    int result;
    if (config.action == decompress)
    {
        if (file_is_encrypted(config.input_filename))
        {
            xor_file(config.input_filename, tmpfile_name, config.password, decrypt);
            result = decompress_file(tmpfile_name, config.output_filename);
        }
        else
        {
            result = decompress_file(config.input_filename, config.output_filename);
        }
    }

    else // compress
    {
        if (config.password)
        {
            result = compress_file(config.input_filename, tmpfile_name, config.word_size);

            if (result == 0)
            {
                xor_file(tmpfile_name, config.output_filename, config.password, encrypt);
            }
        }
        else // no encryption
        {
            result = compress_file(config.input_filename, config.output_filename, config.word_size);
        }
    }

    switch (result)
    {
        case 1:
            fprintf(stderr, "Nie można otworzyć pliku.\n");
            break;
        case 2:
            fprintf(stderr, "Nie można stworzyć pliku %s.\n", argv[2]);
            break;
        case 0:
            if (config.verbose)
            {
                long size_in = get_file_size(config.input_filename);
                long size_out = get_file_size(config.output_filename);

                double comp_percent = (double)size_out / (double)size_in * 100.0;

                printf("Rozmiar pliku wejściowego: %ld bajtów\n",size_in);
                printf("Rozmiar pliku wyjściowego: %ld bajtów\n",size_out);
                printf("Procent kompresji: %lf %%\n",comp_percent);

            }
        default:
            break;
    }
    

    // remove temp file
    return result;
}
