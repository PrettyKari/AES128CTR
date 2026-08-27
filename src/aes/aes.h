/**
 * @file aes.h
 * @brief Internal AES-128 core interface.
 *
 * This header defines the internal AES abstraction layer.
 *
 * It provides:
 *
 *     - AES-128 context management
 *     - AES key expansion
 *     - AES block encryption interface
 *
 * Backend implementations:
 *
 *     software AES
 *     AES-NI AES
 *
 * are hidden behind this interface.
 */

#ifndef AESCRYPT_AES_H
#define AESCRYPT_AES_H


#include <stdint.h>
#include <stddef.h>


/**********************************************************
 * AES Constants
 **********************************************************/

/*
 * AES-128 key size.
 */
#define AESCRYPT_AES128_KEY_SIZE 16U


/*
 * AES block size.
 */
#define AESCRYPT_AES_BLOCK_SIZE 16U


/*
 * AES-128 expanded key size.
 *
 * AES-128:
 *
 *      11 round keys
 *
 *      11 * 16 bytes
 *
 *      = 176 bytes
 */
#define AESCRYPT_AES128_EXPANDED_KEY_SIZE 176U



/**********************************************************
 * AES Context
 **********************************************************/

/*
 * AES-128 execution context.
 *
 * This structure contains internal AES state.
 *
 */
typedef struct
{
    uint8_t round_keys[176U];

} aescrypt_aes128_ctx_t;



/**********************************************************
 * AES Initialization
 **********************************************************/

/*
 * Initialize AES-128 context.
 *
 *
 * Parameters:
 *
 *     ctx
 *          Destination AES context.
 *
 *
 *     key
 *          16-byte AES-128 key.
 *
 *
 */
int aescrypt_aes128_init(
    aescrypt_aes128_ctx_t *ctx,
    const uint8_t key[AESCRYPT_AES128_KEY_SIZE]);



/**********************************************************
 * AES Context Cleanup
 **********************************************************/

/*
 * Clear AES context.
 *
 */
void aescrypt_aes128_clear(
    aescrypt_aes128_ctx_t *ctx);



/**********************************************************
 * AES Block Encryption
 **********************************************************/

/*
 * Encrypt one AES-128 block.
 *
 *
 * Parameters:
 *
 *     ctx
 *          Initialized AES context.
 *
 *
 *     input
 *          Plaintext block.
 *
 *
 *     output
 *          Ciphertext block.
 *
 *
 */
int aescrypt_aes128_encrypt_block(
    const aescrypt_aes128_ctx_t *ctx,
    const uint8_t input[AESCRYPT_AES_BLOCK_SIZE],
    uint8_t output[AESCRYPT_AES_BLOCK_SIZE]);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_AES_H */