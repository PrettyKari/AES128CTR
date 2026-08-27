/**
 * @file nonce.h
 * @brief AES-CTR nonce generation and lifetime management.
 *
 * This header defines the public interface for generating and
 * managing the nonce used by AES-128-CTR.
 *
 * The nonce is cryptographically random and must not be reused
 * with the same AES key.
 *
 * The nonce-management interface is intentionally separated from
 * the AES implementation, key management, backend selection,
 * and file/container layers.
 */

#ifndef AESCRYPT_NONCE_H
#define AESCRYPT_NONCE_H

#include "aes.h"

/**********************************************************
 * Nonce Generation
 **********************************************************/

/*
 * Generate a new cryptographically secure AES-CTR nonce.
 *
 * Implementation guidance:
 *
 * - Generate exactly AESCRYPT_NONCE_SIZE bytes.
 * - The configured nonce size is 8 bytes.
 * - The random source MUST be cryptographically secure.
 * - Do NOT use rand(), srand(), std::rand(), linear congruential
 *   generators, Mersenne Twister, or any other non-cryptographic
 *   PRNG.
 * - The actual platform-specific randomness mechanism should be
 *   hidden behind the random/crypto layer.
 *
 * Expected flow:
 *
 *     aescrypt_nonce_generate()
 *             |
 *             v
 *       crypto random API
 *             |
 *             v
 *        platform CSPRNG
 *             |
 *             v
 *         8 random bytes
 *             |
 *             v
 *       aescrypt_nonce_t
 *
 * IMPORTANT:
 *
 * A nonce must not be reused with the same AES key.
 *
 * The nonce strategy, including how nonce uniqueness is achieved
 * and what assumptions the implementation makes, must be documented
 * in DESIGN.md.
 *
 * Return:
 *     0  on successful nonce generation.
 *     non-zero on failure.
 *
 * On failure, the caller must not assume that `nonce` contains
 * valid nonce material.
 */
int aescrypt_nonce_generate(
    aescrypt_nonce_t *nonce);

/**********************************************************
 * Nonce Clearing
 **********************************************************/

/*
 * Securely clear a nonce from memory.
 *
 * Implementation guidance:
 *
 * - Clear every byte of the nonce.
 * - Use the same secure-memory-clearing strategy used by the
 *   key-management implementation.
 * - Do not rely blindly on an ordinary memset() if the compiler
 *   can optimize the clearing operation away.
 *
 * The function must not attempt to free `nonce`, because the nonce
 * object itself is not dynamically allocated by this API.
 */
void aescrypt_nonce_clear(
    aescrypt_nonce_t *nonce);

/**********************************************************
 * Nonce Validation
 **********************************************************/

/*
 * Validate a nonce object.
 *
 * Implementation guidance:
 *
 * - Check whether the supplied nonce object is valid for use by
 *   the library.
 * - Do NOT attempt to determine whether a nonce is "random enough"
 *   by inspecting its byte values.
 * - Cryptographic randomness is guaranteed by the generation
 *   mechanism, not by checking the resulting bytes.
 *
 * Return:
 *     non-zero if the nonce object is acceptable.
 *     zero otherwise.
 */
int aescrypt_nonce_is_valid(
    const aescrypt_nonce_t *nonce);

/**********************************************************
 * Nonce Copying
 **********************************************************/

/*
 * Copy an AES-CTR nonce.
 *
 * Implementation guidance:
 *
 * - Copy exactly AESCRYPT_NONCE_SIZE bytes.
 * - Keep the operation explicit.
 * - The caller owns both source and destination objects.
 */
int aescrypt_nonce_copy(
    aescrypt_nonce_t *destination,
    const aescrypt_nonce_t *source);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_NONCE_H */