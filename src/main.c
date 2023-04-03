#include <stdio.h>
#include <string.h>
#include "compress.h"
#include <ctype.h>

#include "word.h"
#include "decompress.h"

#include <stdbool.h>

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

void xor_file(FILE* file, const char* cipher_word, bool encrypt)
{
    char key = get_xor(cipher_word);

    if (encrypt)
    {
        char cipher_sign = 0xFF;
        fwrite(&cipher_sign, 1, 1, file);
    }

    char byte;
    //fseek(file, 1, SEEK_SET); // ignore the ff signature
    while (fread(&byte, 1, 1, file))
    {
        byte = byte ^ key;
        fseek(file, -1, SEEK_CUR);
        fwrite(&byte, 1, 1, file);
    }

    fclose(file);
}

typedef struct 
{
    char* input_filename;
    char* output_filename;
    bool decompress;
    int word_size;
    bool encrypt;
    bool verbose;
    char* password;
}Config;


int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [output filename] -d? [compress rate (-O1 | -O2 | -O3)]? -v? encrypt_word?";
    if (argc == 1)
    {
        fprintf(stderr, "%s\n", usage);
        return 1944;
    }

    Config config;

    
    config.input_filename = argv[1];
    config.output_filename = argv[2];
    if (argc >= 4 && strcmp(argv[3], "-d") == 0)
        config.decompress = true;
    else
        config.decompress = false;

    if (config.decompress == false)
    {
        if (strcmp(argv[3], "-O1") == 0)
            config.word_size = 8;
        else if (strcmp(argv[3], "-O2") == 0)
            config.word_size = 12;
        else if (strcmp(argv[3], "-O3") == 0)
            config.word_size = 16;
    }

    if (
        (argc >= 5 && strcmp(argv[4], "-v") == 0) ||
        (argc >= 6 && strcmp(argv[5], "-v") == 0)
    )
        config.verbose = true;
    else
        config.verbose = false;


    if (!config.verbose)
    {
        // 4
        if (argc >= 5)
        {
            config.encrypt = true;
            config.password = argv[4];
        }
        else
        {
            config.encrypt = false;
            config.password = NULL;
        }
    }

    else
    {
        // 5
        if (argc >= 6)
        {
            config.encrypt = true;
            config.password = argv[5];
        }
        else
        {
            config.encrypt = false;
            config.password = NULL;
        }
    }

    FILE* in = fopen(config.input_filename, "rb");
    fseek(in, 0, SEEK_END);
    if (ftell(in) == 0)
    {
        fclose(in);
        fprintf(stderr, "The file is empty.\n");
        return 255;
    }
    fclose(in);

    int result;
    if (config.decompress)
    {
        FILE* input = fopen(config.input_filename, "rb");
        int first_byte = fgetc(input);

        if (first_byte == 0xFF)
            xor_file(input, config.password, false);
        fclose(input);

        result = decompress_file(argv[1], argv[2]);
    }
    else // compress
    {
        result = compress_file(config.input_filename, config.output_filename, config.word_size);
        if(result == 0 && config.encrypt)
        {
            FILE* file = fopen(config.output_filename, "rb");
            xor_file(file, config.password, true);
            fclose(file);
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
                FILE *in;
                FILE *out;

                long size_in,size_out;

                in = fopen(argv[1],"r");
                out = fopen(argv[2],"r");

                fseek(in,0L, SEEK_END);
                fseek(out,0L, SEEK_END);

                size_in = ftell(in);
                size_out = ftell(out);

                fclose(in);
                fclose(out);

                double comp_percent;
                comp_percent = (double)size_out / (double)size_in * 100.0;

                printf("Rozmiar pliku wejściowego: %ld bajtów\n",size_in);
                printf("Rozmiar pliku wyjściowego: %ld bajtów\n",size_out);
                printf("Procent kompresji: %lf procent\n",comp_percent);

            }
        default:
            break;
    }
    
    return result;
}
