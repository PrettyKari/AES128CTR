/**
 * @file ctr.c
 * @brief AES-128 CTR mode implementation.
 */


#include "ctr.h"

#include "aes.h"



/**********************************************************
 * Private Helpers
 **********************************************************/

/*
 * Build AES counter block.
 *
 */
static void ctr_build_block(
    const uint8_t nonce[8U],
    uint64_t counter,
    uint8_t block[16U])
{
    uint8_t index;

    /*
     * Copy nonce.
     */
    for (index = 0U;
         index < 8U;
         index++)
    {
        block[index] =
            nonce[index];
    }

    /*
     * Store counter.
     *
     * Little-endian encoding.
     *
     * Counter occupies bytes 8-15.
     */
    block[8U] =
        (uint8_t)(counter >> 0U);

    block[9U] =
        (uint8_t)(counter >> 8U);

    block[10U] =
        (uint8_t)(counter >> 16U);

    block[11U] =
        (uint8_t)(counter >> 24U);

    block[12U] =
        (uint8_t)(counter >> 32U);

    block[13U] =
        (uint8_t)(counter >> 40U);

    block[14U] =
        (uint8_t)(counter >> 48U);

    block[15U] =
        (uint8_t)(counter >> 56U);
}

/*
 * Increment counter.
 *
 */
static void ctr_increment(
    uint64_t *counter)
{
    /*
     * Increment counter.
     *
     * uint64_t overflow wraps naturally.
     *
     * For production designs:
     *
     * counter exhaustion should be handled.
     */
    (*counter)++;
}



/*
 * Securely clear memory.
 */
static void ctr_secure_zero(
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



/**********************************************************
 * CTR Initialization
 **********************************************************/

int aescrypt_ctr_init(
    aescrypt_ctr_ctx_t *ctx,
    const uint8_t key[16U],
    const uint8_t nonce[8U])
{
    uint8_t index;

    /*
     * Expand AES-128 key.
     */
	aescrypt_aes128_init(
		&ctx->aes,
		key);
		
    /*
     * Store nonce.
     */
    for (index = 0U;
         index < AESCRYPT_CTR_NONCE_SIZE;
         index++)
    {
        ctx->nonce[index] =
            nonce[index];
    }

    /* Initial counter value. */
    ctx->counter = 0U;
	
    return 0;
}



/**********************************************************
 * CTR Encryption / Decryption
 **********************************************************/

int aescrypt_ctr_crypt(
    aescrypt_ctr_ctx_t *ctx,
    const uint8_t *input,
    uint8_t *output,
    size_t size)
{
    uint8_t counter_block[16U];

    uint8_t keystream[16U];

    size_t offset;

    size_t block_size;

    offset = 0U;

    while (offset < size)
    {
        /*
         * Build:
         *
         *      nonce || counter
         */
        ctr_build_block(
            ctx->nonce,
            ctx->counter,
            counter_block);

        /*
         * Encrypt counter block.
         *
         * Result:
         *
         *      keystream
         */
        aescrypt_aes128_encrypt_block(
			&ctx->aes,
			input,
			output);

        /*
         * Determine bytes to process.
         *
         * Last block may be smaller than 16 bytes.
         */
        block_size =
            size - offset;


        if (block_size > AESCRYPT_CTR_BLOCK_SIZE)
        {
            block_size =
                AESCRYPT_CTR_BLOCK_SIZE;
        }
		
		/*
         * XOR input with keystream.
         */
        for (size_t index = 0U;
             index < block_size;
             index++)
        {
            output[offset + index] =
                input[offset + index]
                ^
                keystream[index];
        }

        /*
         * Next counter block.
         */
        ctr_increment(
            &ctx->counter);

        offset += block_size;
	}
    /*
     * Clear temporary keystream.
     */
    ctr_secure_zero(
        keystream,
        sizeof(keystream));


    ctr_secure_zero(
        counter_block,
        sizeof(counter_block));

    return 0;
}



/**********************************************************
 * CTR Cleanup
 **********************************************************/

void aescrypt_ctr_destroy(
    aescrypt_ctr_ctx_t *ctx)
{
    ctr_secure_zero(
		(uint8_t *)&ctx->aes,
		sizeof(ctx->aes));


    ctr_secure_zero(
        ctx->nonce,
        sizeof(ctx->nonce));


    ctx->counter = 0U;
}



/**********************************************************
 * End of File
 **********************************************************/