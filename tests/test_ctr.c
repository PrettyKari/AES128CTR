/**
 * @file test_ctr.c
 * @brief AES-CTR encryption/decryption test.
 */


#include <stdio.h>
#include <string.h>


#include "../src/aes/aes.h"


int main(void)
{
    uint8_t key[16U] = {0};


    uint8_t nonce[8U] = {0};


    uint8_t plaintext[] =
        "AES CTR test message";


    uint8_t encrypted[64U];


    uint8_t decrypted[64U];


    aescrypt_ctr_ctx_t encrypt_ctx;

    aescrypt_ctr_ctx_t decrypt_ctx;



    aescrypt_ctr_init(
        &encrypt_ctx,
        key,
        nonce);



    aescrypt_ctr_crypt(
        &encrypt_ctx,
        plaintext,
        encrypted,
        sizeof(plaintext));



    aescrypt_ctr_init(
        &decrypt_ctx,
        key,
        nonce);



    aescrypt_ctr_crypt(
        &decrypt_ctx,
        encrypted,
        decrypted,
        sizeof(plaintext));



    if (memcmp(
            plaintext,
            decrypted,
            sizeof(plaintext)) != 0)
    {
        printf("CTR test failed\n");

        return 1;
    }



    printf("CTR test passed\n");


    return 0;
}