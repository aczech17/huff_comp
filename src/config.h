#ifndef HUFF_COMP_CONFIG
#define HUFF_COMP_CONFIG

#include <stdbool.h>

typedef enum Action
{
    compress, decompress
}Action;

typedef struct 
{
    char* input_filename;
    char* output_filename;
    Action action;
    int word_size;
    bool verbose;
    char* password;
    char* tmpfile_name;
}Config;

int set_config(Config* config, int argc, char** argv);

#endif // HUFF_COMP_CONFIG
