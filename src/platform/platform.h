/**
 * @file platform.h
 * @brief Platform abstraction interface.
 *
 * This header provides platform-independent access to operating
 * system specific functionality required by the AES library.
 *
 * The purpose of this abstraction is to isolate:
 *
 *      - CPU feature detection
 *      - Cryptographically secure random generation
 *
 * from the rest of the library.
 *
 * Higher-level modules must include this file instead of including
 * Linux or Windows specific headers directly.
 */

#ifndef AESCRYPT_PLATFORM_H
#define AESCRYPT_PLATFORM_H

#include <stddef.h>
#include <stdint.h>


/**********************************************************
 * CPU Feature Detection
 **********************************************************/

/*
 * Determine whether the current processor supports AES-NI.
 *
 * Details:
 *
 * - The actual detection mechanism is platform dependent.
 *
 * - x86/x86-64 platforms perform runtime CPU feature detection.
 *
 * - Unsupported architectures must return zero.
 *
 * Return:
 *
 *     non-zero
 *         AES-NI is available.
 *
 *     zero
 *         AES-NI is unavailable.
 */
int aescrypt_platform_cpu_has_aesni(void);



/**********************************************************
 * Cryptographically Secure Random Generation
 **********************************************************/

/*
 * Generate cryptographically secure random bytes.
 *
 * This API is used for security-sensitive material:
 *
 *     - AES-128 keys
 *     - AES-CTR nonces
 *
 * Parameters:
 *
 *     buffer
 *         Destination buffer.
 *
 *     size
 *         Number of random bytes required.
 *
 * Return:
 *
 *     0
 *         Success.
 *
 *     non-zero
 *         Failure.
 */
int aescrypt_platform_random_bytes(
    uint8_t *buffer,
    size_t size);


/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_PLATFORM_H */