#include "config.h"
#include <string.h>

int set_config(Config* config, int argc, char** argv)
{
    config->tmpfile_name = "temp.xor";

    config->input_filename = argv[1];
    config->output_filename = argv[2];
    if (strcmp(config->output_filename, config->tmpfile_name) == 0)
    {
        return 1;
    }

    if (strcmp(argv[3], "-d") == 0)
        config->action = decompress;
    else
        config->action = compress;

    if (config->action == compress)
    {
        if (strcmp(argv[3], "-O1") == 0)
            config->word_size = 8;
        else if (strcmp(argv[3], "-O2") == 0)
            config->word_size = 12;
        else if (strcmp(argv[3], "-O3") == 0)
            config->word_size = 16;
    }

    if (argc >= 5 && strcmp(argv[4], "-v") == 0)
        config->verbose = true;
    else
        config->verbose = false;

    if (config->verbose)
    {
        if (argc >= 6)
            config->password = argv[5];
        else
            config->password = NULL;
    }
    else
    {
        if (argc >= 5)
            config->password = argv[4];
        else
            config->password = NULL;
    }

    return 0;
}
