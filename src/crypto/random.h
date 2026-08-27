/**
 * @file random.h
 * @brief Cryptographically secure random byte generation interface.
 *
 * This header provides a platform-independent interface for
 * obtaining cryptographically secure random bytes.
 *
 * The implementation delegates the actual random generation to
 * the platform abstraction layer.
 */

#ifndef AESCRYPT_CRYPTO_RANDOM_H
#define AESCRYPT_CRYPTO_RANDOM_H

#include <stddef.h>
#include <stdint.h>


/**********************************************************
 * Random Byte Generation
 **********************************************************/

/*
 * Generate cryptographically secure random bytes.
 *
 * This function is the crypto-layer interface used by:
 *
 *     - AES key generation
 *     - AES-CTR nonce generation
 *
 * The function does not know:
 *
 *     - Linux random APIs
 *     - Windows random APIs
 *     - operating system details
 *
 * The platform layer owns those details.
 *
 *
 * Parameters:
 *
 *     buffer
 *         Destination buffer that receives random bytes.
 *
 *     size
 *         Number of bytes requested.
 *
 *
 * Return:
 *
 *     0
 *         Random bytes successfully generated.
 *
 *     non-zero
 *         Random generation failed.
 *
 */
int aescrypt_random_bytes(
    uint8_t *buffer,
    size_t size);


/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_CRYPTO_RANDOM_H */