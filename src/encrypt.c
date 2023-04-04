#include "encrypt.h"
#include <stdio.h>
#include <ctype.h>

typedef enum
{
    encrypt, decrypt
}EncryptionAction;

static char get_xor(const char* word)
{
    char key = 0;
    int i;
    for (i = 0; isalnum(word[i]); i++)
    {
        key = key ^ word[i];
    }

    return key;
}

static void xor_file(const char* input_filename, const char* output_filename, const char* encrypt_word, EncryptionAction action)
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

void encrypt_file(const char* input_filename, const char* output_filename, const char* encrypt_word)
{
    xor_file(input_filename, output_filename, encrypt_word, encrypt);
}

void decrypt_file(const char* input_filename, const char* output_filename, const char* encrypt_word)
{
    xor_file(input_filename, output_filename, encrypt_word, decrypt);
}
