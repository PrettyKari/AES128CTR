/**
 * @file test_aes.c
 * @brief AES-128 known answer test.
 */


#include <stdio.h>
#include <stdint.h>


#include "aescrypt/aes.h"



static int compare_bytes(
    const uint8_t *a,
    const uint8_t *b,
    size_t size)
{
    size_t index;


    for (index = 0U;
         index < size;
         index++)
    {
        if (a[index] != b[index])
        {
            return -1;
        }
    }


    return 0;
}



int main(void)
{
    uint8_t key[16U] =
    {
        0x00,0x01,0x02,0x03,
        0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,
        0x0c,0x0d,0x0e,0x0f
    };


    uint8_t plaintext[16U] =
    {
        0x00,0x11,0x22,0x33,
        0x44,0x55,0x66,0x77,
        0x88,0x99,0xaa,0xbb,
        0xcc,0xdd,0xee,0xff
    };


    uint8_t expected[16U] =
    {
        0x69,0xc4,0xe0,0xd8,
        0x6a,0x7b,0x04,0x30,
        0xd8,0xcd,0xb7,0x80,
        0x70,0xb4,0xc5,0x5a
    };


    uint8_t output[16U];



    aescrypt_aes128_encrypt_block(
        plaintext,
        output,
        key);



    if (compare_bytes(
            output,
            expected,
            16U) != 0)
    {
        printf("AES test failed\n");

        return 1;
    }



    printf("AES test passed\n");


    return 0;
}