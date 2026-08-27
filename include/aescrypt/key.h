/**
 * @file key.h
 * @brief AES-128 key generation and lifetime management.
 *
 * This header defines the public interface for creating and
 * managing AES-128 key material.
 *
 * Key generation must use a cryptographically secure random
 * number generator.
 *
 * The key-management interface is intentionally separated from
 * the AES implementation, backend selection, and file/container
 * layers.
 */

#ifndef AESCRYPT_KEY_H
#define AESCRYPT_KEY_H

#include "aes.h"



typedef struct aescrypt_key
{
    uint8_t bytes[AESCRYPT_AES128_KEY_SIZE];

} aescrypt_key_t;

/**********************************************************
 * Key Generation
 **********************************************************/

/*
 * Generate a new cryptographically secure AES-128 key.
 *
 * Implementation guidance:
 *
 * - Generate exactly AESCRYPT_AES128_KEY_SIZE bytes.
 * - The random source MUST be cryptographically secure.
 * - Do NOT use rand(), srand(), std::rand(), linear congruential
 *   generators, Mersenne Twister, or any other non-cryptographic
 *   PRNG for key generation.
 * - The actual platform-specific randomness mechanism should be
 *   hidden behind the random/crypto layer.
 *
 * Expected flow:
 *
 *     aescrypt_key_generate()
 *             |
 *             v
 *       crypto random API
 *             |
 *             v
 *       platform CSPRNG
 *             |
 *             v
 *       16 random bytes
 *             |
 *             v
 *       aescrypt_key_t
 *
 * Return:
 *     0  on successful key generation.
 *     non-zero on failure.
 *
 * On failure, the caller must not assume that `key` contains
 * valid key material.
 */
int aescrypt_key_generate(
    aescrypt_key_t *key);

/**********************************************************
 * Key Clearing
 **********************************************************/

/*
 * Securely clear an AES-128 key from memory.
 *
 * Implementation guidance:
 *
 * - Clear every byte of the key.
 * - Do not rely blindly on an ordinary memset() if the compiler
 *   can optimize the clearing operation away.
 * - The implementation should use an appropriate secure-clearing
 *   mechanism for the supported platforms.
 *
 * This function should be safe to call:
 *
 *     - after successful key generation
 *     - after encryption/decryption is finished
 *     - when abandoning a key because of an error
 *
 * The function must not attempt to free `key`, because the key
 * object itself is not dynamically allocated by this API.
 */
void aescrypt_key_clear(
    aescrypt_key_t *key);

/**********************************************************
 * Key Validation
 **********************************************************/

/*
 * Validate an AES-128 key object.
 *
 * Implementation guidance:
 *
 * - Determine what constitutes a valid key object for this API.
 * - Do not attempt to validate cryptographic "strength" by
 *   inspecting the bytes; a valid AES key may contain any
 *   128-bit value.
 * - The primary purpose of this function is API/state validation,
 *   not cryptographic analysis.
 *
 * Return:
 *     non-zero if the key object is acceptable.
 *     zero otherwise.
 */
int aescrypt_key_is_valid(
    const aescrypt_key_t *key);

/**********************************************************
 * Key Copying
 **********************************************************/

/*
 * Copy an AES-128 key.
 *
 * Implementation guidance:
 *
 * - Copy exactly AESCRYPT_AES128_KEY_SIZE bytes.
 * - Keep in mind that every additional copy increases the amount
 *   of time raw key material exists in process memory.
 * - This function should therefore remain explicit rather than
 *   providing unnecessary implicit key-copying abstractions.
 *
 * The caller owns both source and destination objects.
 */
int aescrypt_key_copy(
    aescrypt_key_t *destination,
    const aescrypt_key_t *source);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_KEY_H */