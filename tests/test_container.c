/**
 * @file test_container.c
 * @brief Container encryption/decryption API test.
 */


#include <stdio.h>
#include <string.h>


#include "aescrypt/key.h"
#include "aescrypt/container.h"



int main(void)
{
    aescrypt_key_t key;


    FILE *file;



    file =
        fopen(
            "container_input.txt",
            "wb");


    if (file == NULL)
    {
        return 1;
    }


    fwrite(
        "container test",
        1U,
        14U,
        file);


    fclose(file);



    aescrypt_key_generate(
        &key);



    if (aescrypt_encrypt_file(
            "container_input.txt",
            "container_test.aes",
            &key) != 0)
    {
        return 1;
    }



    if (aescrypt_decrypt_file(
            "container_test.aes",
            "container_output.txt",
            &key) != 0)
    {
        return 1;
    }



    aescrypt_key_clear(
        &key);



    printf(
        "Container test passed\n");


    return 0;
}