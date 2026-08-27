/**
 * @file windows_random.h
 * @brief Windows cryptographically secure random interface.
 *
 * This header defines the Windows-specific interface for obtaining
 * cryptographically secure random bytes.
 *
 * The implementation is used by higher-level key and nonce
 * generation code through the common platform abstraction.
 */

#ifndef AESCRYPT_WINDOWS_RANDOM_H
#define AESCRYPT_WINDOWS_RANDOM_H

#include <stddef.h>
#include <stdint.h>

/**********************************************************
 * Cryptographically Secure Random Bytes
 **********************************************************/

/*
 * Generate cryptographically secure random bytes.
 *
 * The function is intentionally generic because the same
 * platform CSPRNG will be used for:
 *
 *     - AES-128 key generation
 *     - AES-CTR nonce generation
 *
 * Parameters:
 *
 *     buffer
 *         Destination buffer that receives random bytes.
 *
 *     size
 *         Number of random bytes requested.
 *
 * Return:
 *     0       on success.
 *     non-zero on failure.
 *
 * On failure, the caller must not assume that the complete
 * requested buffer contains valid random data.
 */
int aescrypt_windows_random_bytes(
    uint8_t *buffer,
    size_t size);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_WINDOWS_RANDOM_H */.0