#include <stdio.h>
#include <string.h>
#include "compress.h"


int main(int argc, char** argv)
{
    const char* usage = "comp [input filename] [output filename] [compress rate (-O1 | -O2 | -O3)]";
    if (argc < 4)
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

    compress_file(argv[1], argv[2], word_size);

    
    
    return 0;
}
