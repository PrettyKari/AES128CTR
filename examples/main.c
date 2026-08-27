/**
 * @file main.c
 * @brief AESCrypt example application.
 */


#include <stdio.h>

#include "aescrypt/container.h"
#include "aescrypt/key.h"
#include "../src/container/writer.h"
#include "../src/container/reader.h"



/**********************************************************
 * Example Application
 **********************************************************/

int main(void)
{
    aescrypt_key_t key;


    /*
     * Generate AES-128 key.
     *
     * In a real application:
     *
     * - load from secure storage
     * - do not generate every run
     */
    if (aescrypt_key_generate(&key) != 0)
    {
        printf("key generation failed\n");

        return 1;
    }



    /*
     * Encrypt example file.
     */
    if (aescrypt_container_write(
            "input.txt",
            "encrypted.aes",
            &key) != 0)
    {
        printf("encryption failed\n");

        aescrypt_key_clear(
            &key);

        return 1;
    }



    /*
     * Decrypt example file.
     */
    if (aescrypt_container_read(
            "encrypted.aes",
            "output.txt",
            &key) != 0)
    {
        printf("decryption failed\n");

        aescrypt_key_clear(
            &key);

        return 1;
    }



    /*
     * Remove key material.
     */
    aescrypt_key_clear(
        &key);



    printf("operation completed\n");


    return 0;
}