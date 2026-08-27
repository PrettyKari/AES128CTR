/**
 * @file writer.h
 * @brief Encrypted container writer interface.
 *
 * This header defines the API used to create encrypted
 * AESCrypt container files.
 *
 * The writer layer coordinates:
 *
 *      - input file reading
 *      - container creation
 *      - AES-CTR encryption
 *      - output file writing
 *
 * Cryptographic operations are implemented by lower layers.
 */


#ifndef AESCRYPT_CONTAINER_WRITER_H
#define AESCRYPT_CONTAINER_WRITER_H


#include <stdint.h>
#include <stddef.h>
#include "../../include/aescrypt/key.h"


/**********************************************************
 * Writer Configuration
 **********************************************************/

/*
 * Default processing buffer size.
 *
 */
#define AESCRYPT_WRITER_BUFFER_SIZE 4096U



/**********************************************************
 * Writer Operation
 **********************************************************/

/*
 * Encrypt an input file into an AESCrypt container.
 *
 *
 * Parameters:
 *
 *      input_path
 *          Path to plaintext input file.
 *
 *
 *      output_path
 *          Path where encrypted container is created.
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
 * Expected flow:
 *
 *
 * open input file
 *
 *        |
 *        v
 *
 * determine plaintext size
 *
 *        |
 *        v
 *
 * generate nonce
 *
 *        |
 *        v
 *
 * create container header
 *
 *        |
 *        v
 *
 * write header
 *
 *        |
 *        v
 *
 * initialize AES-CTR
 *
 *        |
 *        v
 *
 * read/encrypt/write chunks
 *
 *
 * The function must not expose raw key material.
 */
int aescrypt_container_write(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CONTAINER_WRITER_H */