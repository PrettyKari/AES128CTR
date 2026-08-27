/**
 * @file container.h
 * @brief AESCrypt encrypted container interface.
 *
 * This header defines the public API for creating and
 * reading AESCrypt encrypted container files.
 *
 * The container format internally uses:
 *
 *      AES-128-CTR
 *
 * The container implementation manages:
 *
 *      - metadata creation
 *      - nonce handling
 *      - encryption/decryption workflow
 *
 * Internal format details are hidden from users.
 */


#ifndef AESCRYPT_CONTAINER_H
#define AESCRYPT_CONTAINER_H


#include <stdint.h>


#include "key.h"



/**********************************************************
 * Container Operations
 **********************************************************/

/*
 * Encrypt a file into an AESCrypt container.
 *
 *
 * Parameters:
 *
 *      input_path
 *          Path to plaintext input file.
 *
 *
 *      output_path
 *          Path for generated encrypted container.
 *
 *
 *      key
 *          AES-128 encryption key.
 *
 *
 * Return:
 *
 *      0
 *          Success.
 *
 *
 *      non-zero
 *          Failure.
 *
 *
 * Processing:
 *
 *
 *      plaintext file
 *
 *              |
 *              v
 *
 *      AES-128-CTR encryption
 *
 *              |
 *              v
 *
 *      encrypted container
 */
int aescrypt_encrypt_file(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key);



/*
 * Decrypt an AESCrypt container into a file.
 *
 *
 * Parameters:
 *
 *      input_path
 *          Path to encrypted container.
 *
 *
 *      output_path
 *          Path for restored plaintext file.
 *
 *
 *      key
 *          AES-128 encryption key.
 *
 *
 * Return:
 *
 *      0
 *          Success.
 *
 *
 *      non-zero
 *          Failure.
 *
 *
 * Processing:
 *
 *
 *      encrypted container
 *
 *              |
 *              v
 *
 *      AES-128-CTR decryption
 *
 *              |
 *              v
 *
 *      plaintext file
 */
int aescrypt_decrypt_file(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CONTAINER_H */