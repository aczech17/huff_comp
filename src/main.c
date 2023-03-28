#include <stdio.h>
#include <string.h>
#include "compress.h"
#include <ctype.h>

#include "word.h"

char get_xor(char* word)
{
    char key = 0;
    int i;
    for (i = 0; isalnum(word[i]); i++)
    {
        key = key ^ word[i];
    }

    return key;
}

int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [output filename] [compress rate (-O1 | -O2 | -O3)] -v? [-s ciph_word]?";
    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        fprintf(stderr, "%s\n", usage);
        return 1;
    }

    int word_size;
    if (strcmp(argv[3], "-O1") == 0)
        word_size = 8;
    else if (strcmp(argv[3], "-O2") == 0)
        word_size = 12;
    else if (strcmp(argv[3], "-O3") == 0)
        word_size = 16;
    else
    {
        fprintf(stderr, "Bad argument\n");
        return 2;
    }

    int compress_result = compress_file(argv[1], argv[2], word_size);
    switch (compress_result)
    {
        case 1:
            fprintf(stderr, "Nie można otworzyć pliku.\n");
            break;
        case 2:
            fprintf(stderr, "Nie można stworzyć pliku.\n");
            break;
        case 0:
            if (argc >= 5 && strcmp(argv[4], "-v") == 0){
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

            char* cipher_word = NULL;
            if (argc >= 5 && strcmp(argv[4], "-s") == 0)
            {
                cipher_word = argv[5];
            }
            else if (argc >= 7 && strcmp(argv[5], "-s") == 0)
            {
                cipher_word = argv[6];
            }

            if (cipher_word)
            {
                char key = get_xor(cipher_word);

                FILE* file = fopen(argv[2], "rb+");

                char cipher_sign = 0xFF;
                fwrite(&cipher_sign, 1, 1, file);


                char byte;
                fseek(file, 1, SEEK_SET); // ignore the ff signature
                while (fread(&byte, 1, 1, file))
                {
                    byte = byte ^ key;
                    fseek(file, -1, SEEK_CUR);
                    fwrite(&byte, 1, 1, file);
                }

                fclose(file);
            }

        default:
            break;
    }

    
    return compress_result;
}
