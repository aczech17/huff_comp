#include <stdio.h>
#include <string.h>
#include "compress.h"


int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [output filename] [compress rate (-O1 | -O2 | -O3)]";
    if (argc != 6)
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
            if (strcmp(argv[4], "-v") == 0){
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

                printf("Rozmiar pliku 1: %ld \n",size_in);
                printf("Rozmiar pliku 2: %ld \n",size_out);
            }
        default:
            break;
    }

    
    return compress_result;
}