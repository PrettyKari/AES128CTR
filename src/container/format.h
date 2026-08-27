/**
 * @file format.h
 * @brief AESCrypt container file format definition.
 *
 * This header defines the binary layout of encrypted files.
 *
 * The container format stores:
 *
 *      - file identification
 *      - version information
 *      - encryption parameters
 *      - nonce
 *      - encrypted payload metadata
 *
 * The actual AES implementation is hidden from this layer.
 */


#ifndef AESCRYPT_CONTAINER_FORMAT_H
#define AESCRYPT_CONTAINER_FORMAT_H


#include <stdint.h>
#include <stddef.h>



/**********************************************************
 * Container Constants
 **********************************************************/

/*
 * File magic identifier.
 *
 * Used to recognize AESCrypt files.
 */
#define AESCRYPT_CONTAINER_MAGIC_SIZE 8U



/*
 * Current container version.
 */
#define AESCRYPT_CONTAINER_VERSION 1U



/*
 * Header size.
 *
 * Must match the serialized header size.
 */
#define AESCRYPT_CONTAINER_HEADER_SIZE 32U



/**********************************************************
 * Container Header
 **********************************************************/

/*
 * Container header.
 *
 * This structure represents metadata stored before
 * encrypted payload data.
 *
 */
typedef struct
{
    /*
     * File identification bytes.
     */
    uint8_t magic[AESCRYPT_CONTAINER_MAGIC_SIZE];


    /*
     * Container format version.
     */
    uint8_t version;


    /*
     * Reserved bytes for future extensions.
     */
    uint8_t reserved[7U];


    /*
     * AES-CTR nonce.
     *
     * 8 bytes according to project design.
     */
    uint8_t nonce[8U];


    /*
     * Original plaintext size.
     *
     * Required for restoring exact file size.
     */
    uint64_t plaintext_size;


} aescrypt_container_header_t;



/**********************************************************
 * Header Operations
 **********************************************************/

/*
 * Initialize a container header.
 *
 */
void aescrypt_container_header_init(
    aescrypt_container_header_t *header,
    const uint8_t nonce[8U],
    uint64_t plaintext_size);



/*
 * Serialize container header.
 *
 * Converts structure representation into binary format.
 *
 */
int aescrypt_container_header_encode(
    const aescrypt_container_header_t *header,
    uint8_t output[AESCRYPT_CONTAINER_HEADER_SIZE]);



/*
 * Deserialize container header.
 *
 * Converts binary data into structure representation.
 *
 */
int aescrypt_container_header_decode(
    aescrypt_container_header_t *header,
    const uint8_t input[AESCRYPT_CONTAINER_HEADER_SIZE]);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CONTAINER_FORMAT_H */