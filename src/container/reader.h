/**
 * @file reader.h
 * @brief Encrypted container reader interface.
 *
 * This header defines the API used to decrypt
 * AESCrypt container files.
 */

#ifndef AESCRYPT_CONTAINER_READER_H
#define AESCRYPT_CONTAINER_READER_H


#include <stdint.h>

#include "../crypto/key.h"



/**********************************************************
 * Reader Configuration
 **********************************************************/

#define AESCRYPT_READER_BUFFER_SIZE 4096U



/**********************************************************
 * Container Reader
 **********************************************************/

/*
 * Decrypt an AESCrypt container file.
 *
 *
 * Parameters:
 *
 *      input_path
 *          Encrypted container path.
 *
 *
 *      output_path
 *          Destination plaintext path.
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
 *      non-zero
 *          Failure.
 *
 *
 * Processing flow:
 *
 *
 *      read container header
 *
 *              |
 *              v
 *
 *      extract nonce
 *
 *              |
 *              v
 *
 *      initialize AES-CTR
 *
 *              |
 *              v
 *
 *      decrypt ciphertext chunks
 *
 *              |
 *              v
 *
 *      write plaintext
 */
int aescrypt_container_read(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CONTAINER_READER_H */