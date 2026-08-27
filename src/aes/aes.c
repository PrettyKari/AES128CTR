/**
 * @file aes.c
 * @brief AES-128 core implementation.
 *
 * This file provides backend-independent AES functionality.
 *
 * It performs:
 *
 *     - key expansion
 *     - context management
 *     - backend dispatch
 *
 * It does not implement AES rounds.
 */


#include "aes.h"

#include "software.h"
#include "aesni.h"
#include "../backend/dispatch.h"



/**********************************************************
 * AES Constants
 **********************************************************/

/*
 * AES round constants.
 *
 * Used during AES-128 key expansion.
 */
static const uint8_t aes_rcon[10U] =
{
    0x01U,
    0x02U,
    0x04U,
    0x08U,
    0x10U,
    0x20U,
    0x40U,
    0x80U,
    0x1BU,
    0x36U
};



/**********************************************************
 * Internal Helpers
 **********************************************************/

/**********************************************************
 * Secure Memory Clear
 **********************************************************/

static void aes_secure_zero(
    uint8_t *buffer,
    size_t size)
{
    volatile uint8_t *ptr;

    ptr = buffer;


    while (size > 0U)
    {
        *ptr = 0U;

        ptr++;

        size--;
    }
}

static void aes_rot_word(
    uint8_t word[4])
{
    uint8_t temp;


    temp = word[0U];


    word[0U] = word[1U];
    word[1U] = word[2U];
    word[2U] = word[3U];
    word[3U] = temp;
}



static void aes_sub_word(
    uint8_t word[4])
{
    uint8_t index;


    for (index = 0U;
         index < 4U;
         index++)
    {
        word[index] =
            aescrypt_aes_sbox_lookup(
                word[index]);
    }
}

/**********************************************************
 * AES-128 Key Expansion
 **********************************************************/

static void aes_expand_key(
    const uint8_t key[16],
    uint8_t round_keys[176])
{
    uint8_t temp[4];

    uint32_t generated;

    uint8_t rcon_index;

    uint8_t index;



    /*
     * Copy original key.
     *
     * Round key 0.
     */
    for (index = 0U;
         index < 16U;
         index++)
    {
        round_keys[index] = key[index];
    }



    generated = 16U;

    rcon_index = 0U;


    while (generated < 176U)
    {
        /*
         * Load previous word.
         */
        temp[0U] =
            round_keys[generated - 4U];

        temp[1U] =
            round_keys[generated - 3U];

        temp[2U] =
            round_keys[generated - 2U];

        temp[3U] =
            round_keys[generated - 1U];



        /*
         * Every fourth word:
         *
         * RotWord
         * SubWord
         * Rcon XOR
         */
        if ((generated % 16U) == 0U)
        {
            aes_rot_word(temp);

            aes_sub_word(temp);

            temp[0U] ^=
                aes_rcon[rcon_index];

            rcon_index++;
        }



        /*
         * Generate new word.
         */
        for (index = 0U;
             index < 4U;
             index++)
        {
            round_keys[generated] =
                round_keys[generated - 16U]
                ^
                temp[index];


            generated++;
        }
    }
}

/**********************************************************
 * AES Initialization
 **********************************************************/

int aescrypt_aes128_init(
    aescrypt_aes128_ctx_t *ctx,
    const uint8_t key[AESCRYPT_AES128_KEY_SIZE])
{
    aes_expand_key(
        key,
        ctx->round_keys);


    return 0;

}



/**********************************************************
 * AES Context Cleanup
 **********************************************************/

void aescrypt_aes128_clear(
    aescrypt_aes128_ctx_t *ctx)
{
    aes_secure_zero(
        ctx->round_keys,
        AESCRYPT_AES128_EXPANDED_KEY_SIZE);
}



/**********************************************************
 * AES Block Encryption
 **********************************************************/

int aescrypt_aes128_encrypt_block(
    const aescrypt_aes128_ctx_t *ctx,
    const uint8_t input[AESCRYPT_AES_BLOCK_SIZE],
    uint8_t output[AESCRYPT_AES_BLOCK_SIZE])
{
    aescrypt_encrypt_block_fn encrypt;

    encrypt =
        aescrypt_backend_get_encrypt_function();

    encrypt(
        input,
        output,
        ctx->round_keys);

    return 0;
}



/**********************************************************
 * End of File
 **********************************************************/