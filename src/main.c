#include <stdio.h>
#include "compress.h"
#include "decompress.h"
#include "config.h"
#include "encrypt.h"

int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [output filename] (-d | [compress rate (-O1 | -O2 | -O3)]) -v? encrypt_word?";
    /*                   0    1                2                 3                                         4  (4 | 5)   */

    if (argc < 4)
    {
        fprintf(stderr, "%s\n", usage);
        return 1944;
    }

    Config config;
    if (set_config(&config, argc, argv) == 1)
    {
        fprintf(stderr, "Zarezerwowana nazwa pliku: %s\n", config.tmpfile_name);
        return 1;
    }
    
    if (get_file_size(config.input_filename) == 0)
    {
        FILE* empty_file = fopen(config.output_filename, "wb");
        fclose(empty_file);
        return 0;
    }

    int result;
    if (config.action == decompress)
    {
        if (file_is_encrypted(config.input_filename))
        {
            bool decrypt_correct = decrypt_file(config.input_filename, config.tmpfile_name, config.password);
            if (!decrypt_correct)
            {
                fprintf(stderr, "Decryption incorrect.\n");
                return 3;
            }
            result = decompress_file(config.tmpfile_name, config.output_filename);
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
            result = compress_file(config.input_filename, config.tmpfile_name, config.word_size);

            if (result == 0)
            {
                encrypt_file(config.tmpfile_name, config.output_filename, config.password);
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
            fprintf(stderr, "Nie można otworzyć pliku %s.\n", config.output_filename);
            break;
        case 2:
            fprintf(stderr, "Nie można stworzyć pliku %s.\n", config.output_filename);
            break;
        case 0:
            if (config.action == compress && config.verbose)
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
    
    remove(config.tmpfile_name);
    return result;
}
