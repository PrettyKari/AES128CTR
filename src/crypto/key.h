/**
 * @file key.h
 * @brief AES-128 key management interface.
 *
 * This header defines creation, validation, copying, and
 * destruction operations for AES-128 key material.
 *
 * Key generation is separated from the AES implementation.
 * This module only manages raw AES key bytes.
 */

#ifndef AESCRYPT_CRYPTO_KEY_H
#define AESCRYPT_CRYPTO_KEY_H


#include "../../include/aescrypt/aes.h"
#include "../../include/aescrypt/key.h"
/**********************************************************
 * Key Generation
 **********************************************************/

/*
 * Parameters:
 *
 *     key
 *         Destination key object.
 *
 *
 * Return:
 *
 *     0
 *         Key generated successfully.
 *
 *     non-zero
 *         Generation failed.
 *
 *
 * Failure:
 *
 * - The caller must not assume the key contains valid data.
 */
int aescrypt_key_generate(
    aescrypt_key_t *key);



/**********************************************************
 * Key Clearing
 **********************************************************/

/*
 * Securely clear key material from memory.
 *
 * The function:
 *
 *     - does not free memory.
 *     - does not release the key object.
 *     - only destroys the stored key bytes.
 */
void aescrypt_key_clear(
    aescrypt_key_t *key);


/**********************************************************
 * Key Copy
 **********************************************************/

/*
 * Explicitly copy AES-128 key material.
 *
 * Return:
 *
 *     0
 *         Success.
 *
 *     non-zero
 *         Failure.
 */
int aescrypt_key_copy(
    aescrypt_key_t *destination,
    const aescrypt_key_t *source);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CRYPTO_KEY_H */