/**
 * @file ctr.h
 * @brief AES-128 CTR mode implementation interface.
 *
 * This header defines AES-128 counter mode encryption.
 *
 * CTR mode provides the same operation for encryption
 * and decryption.
 *
 * The counter block format used by this project:
 *
 *      +----------------+----------------+
 *      | 8-byte nonce   | 8-byte counter |
 *      +----------------+----------------+
 *
 * Total:
 *
 *      16-byte AES block
 */


#ifndef AESCRYPT_CTR_H
#define AESCRYPT_CTR_H


#include <stdint.h>
#include <stddef.h>
#include "./aes.h"

/**********************************************************
 * CTR Constants
 **********************************************************/

/*
 * AES block size.
 */
#define AESCRYPT_CTR_BLOCK_SIZE 16U



/*
 * Nonce size.
 */
#define AESCRYPT_CTR_NONCE_SIZE 8U



/*
 * Counter size.
 */
#define AESCRYPT_CTR_COUNTER_SIZE 8U



/**********************************************************
 * CTR Context
 **********************************************************/

/*
 * AES-CTR runtime context.
 *
 * This structure keeps:
 *
 *      - AES expanded key
 *      - nonce
 *      - current counter value
 *
 */
typedef struct
{
	aescrypt_aes128_ctx_t aes;

    /*
     * CTR nonce.
     *
     * Fixed for the lifetime of this context.
     */
    uint8_t nonce[8U];


    /*
     * Current counter value.
     *
     * Incremented after every generated block.
     */
    uint64_t counter;


} aescrypt_ctr_ctx_t;



/**********************************************************
 * CTR Initialization
 **********************************************************/

/*
 * Initialize AES-128-CTR context.
 *
 *
 * Parameters:
 *
 *      ctx
 *          CTR state object.
 *
 *
 *      key
 *          AES-128 key.
 *
 *
 *      nonce
 *          8-byte nonce.
 *
 *
 * Counter initial value:
 *
 *      0
 *
 */
int aescrypt_ctr_init(
    aescrypt_ctr_ctx_t *ctx,
    const uint8_t key[16U],
    const uint8_t nonce[8U]);



/**********************************************************
 * CTR Encryption / Decryption
 **********************************************************/

/*
 * Process data using AES-CTR.
 *
 * Encryption:
 *
 *      plaintext XOR keystream
 *
 *
 * Decryption:
 *
 *      ciphertext XOR keystream
 *
 *
 * The same function performs both.
 *
 *
 * Parameters:
 *
 *      input
 *          Input data buffer.
 *
 *
 *      output
 *          Output buffer.
 *
 *
 *      size
 *          Number of bytes to process.
 *
 *
 * Handle final partial block correctly.
 */
int aescrypt_ctr_crypt(
    aescrypt_ctr_ctx_t *ctx,
    const uint8_t *input,
    uint8_t *output,
    size_t size);



/**********************************************************
 * CTR Cleanup
 **********************************************************/

/*
 * Destroy CTR context.
 *
 */
void aescrypt_ctr_destroy(
    aescrypt_ctr_ctx_t *ctx);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CTR_H */