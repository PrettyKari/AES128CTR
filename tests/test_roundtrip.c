/**
 * @file test_roundtrip.c
 * @brief Full encryption/decryption roundtrip test.
 */


#include <stdio.h>
#include <string.h>


#include "aescrypt/key.h"
#include "aescrypt/container.h"



int main(void)
{
    aescrypt_key_t key;


    FILE *file;


    char original[] =
        "roundtrip validation";



    char buffer[64U];



    file =
        fopen(
            "roundtrip_input.txt",
            "wb");


    fwrite(
        original,
        1U,
        strlen(original),
        file);


    fclose(file);



    aescrypt_key_generate(
        &key);



    aescrypt_encrypt_file(
        "roundtrip_input.txt",
        "roundtrip.aes",
        &key);



    aescrypt_decrypt_file(
        "roundtrip.aes",
        "roundtrip_output.txt",
        &key);



    aescrypt_key_clear(
        &key);



    file =
        fopen(
            "roundtrip_output.txt",
            "rb");



    fread(
        buffer,
        1U,
        strlen(original),
        file);


    fclose(file);



    if (memcmp(
            buffer,
            original,
            strlen(original)) != 0)
    {
        printf(
            "Roundtrip failed\n");

        return 1;
    }



    printf(
        "Roundtrip passed\n");


    return 0;
}