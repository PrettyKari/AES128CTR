/**
 * @file format.c
 * @brief AESCrypt container format implementation.
 */


#include "format.h"



/**********************************************************
 * Container Constants
 **********************************************************/

static const uint8_t container_magic[8U] =
{
    'A',
    'E',
    'S',
    'C',
    'R',
    'Y',
    'P',
    'T'
};



/**********************************************************
 * Header Initialization
 **********************************************************/

void aescrypt_container_header_init(
    aescrypt_container_header_t *header,
    const uint8_t nonce[8U],
    uint64_t plaintext_size)
{
    uint8_t index;

	/*
     * Set magic identifier.
     */
    for (index = 0U;
         index < AESCRYPT_CONTAINER_MAGIC_SIZE;
         index++)
    {
        header->magic[index] =
            container_magic[index];
    }

    /*
     * Set container version.
     */
    header->version =
        AESCRYPT_CONTAINER_VERSION;

    /*
     * Clear reserved bytes.
     *
     * Reserved space must have deterministic values.
     */
    for (index = 0U;
         index < 7U;
         index++)
    {
        header->reserved[index] = 0U;
    }
	
	/*
     * Store nonce.
     */
    for (index = 0U;
         index < 8U;
         index++)
    {
        header->nonce[index] =
            nonce[index];
    }

    /*
     * Store original plaintext size.
     */
    header->plaintext_size =
        plaintext_size;
}



/**********************************************************
 * Header Encoding
 **********************************************************/

int aescrypt_container_header_encode(
    const aescrypt_container_header_t *header,
    uint8_t output[32U])
{
    uint8_t index;

    /*
     * Magic.
     */
    for (index = 0U;
         index < 8U;
         index++)
    {
        output[index] =
            header->magic[index];
    }

    /*
     * Version.
     */
    output[8U] =
        header->version;

    /*
     * Reserved bytes.
     */
    for (index = 0U;
         index < 7U;
         index++)
    {
        output[9U + index] =
            header->reserved[index];
    }
	
	/*
     * Nonce.
     */
    for (index = 0U;
         index < 8U;
         index++)
    {
        output[16U + index] =
            header->nonce[index];
    }

    /*
     * Plaintext size.
     *
     * Little-endian encoding.
     */
    output[24U] =
        (uint8_t)(header->plaintext_size >> 0U);

    output[25U] =
        (uint8_t)(header->plaintext_size >> 8U);

    output[26U] =
        (uint8_t)(header->plaintext_size >> 16U);

    output[27U] =
        (uint8_t)(header->plaintext_size >> 24U);

    output[28U] =
        (uint8_t)(header->plaintext_size >> 32U);

    output[29U] =
        (uint8_t)(header->plaintext_size >> 40U);

    output[30U] =
        (uint8_t)(header->plaintext_size >> 48U);

    output[31U] =
        (uint8_t)(header->plaintext_size >> 56U);

    return 0;
}



/**********************************************************
 * Header Decoding
 **********************************************************/

int aescrypt_container_header_decode(
    aescrypt_container_header_t *header,
    const uint8_t input[32U])
{

    uint8_t index;

    /*
     * Validate magic identifier.
     */
    for (index = 0U;
         index < AESCRYPT_CONTAINER_MAGIC_SIZE;
         index++)
    {
        if (input[index] != container_magic[index])
        {
            return -1;
        }
    }

    /*
     * Validate version.
     */
    if (input[8U] != AESCRYPT_CONTAINER_VERSION)
    {
        return -1;
    }

    /*
     * Restore magic.
     */
    for (index = 0U;
         index < 8U;
         index++)
    {
        header->magic[index] =
            input[index];
    }

    /*
     * Restore version.
     */
    header->version =
        input[8U];

    /*
     * Restore reserved bytes.
     */
    for (index = 0U;
         index < 7U;
         index++)
    {
        header->reserved[index] =
            input[9U + index];
    }

    /*
     * Restore nonce.
     */
    for (index = 0U;
         index < 8U;
         index++)
    {
        header->nonce[index] =
            input[16U + index];
    }
	
	/*
     * Restore plaintext size.
     *
     * Little-endian decoding.
     */
    header->plaintext_size =
          ((uint64_t)input[24U] << 0U)
        | ((uint64_t)input[25U] << 8U)
        | ((uint64_t)input[26U] << 16U)
        | ((uint64_t)input[27U] << 24U)
        | ((uint64_t)input[28U] << 32U)
        | ((uint64_t)input[29U] << 40U)
        | ((uint64_t)input[30U] << 48U)
        | ((uint64_t)input[31U] << 56U);

    return 0;
}