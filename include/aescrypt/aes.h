/**
 * @file aes.h
 * @brief Public AES-128 encryption interface.
 *
 * This header defines the public cryptographic interface exposed by
 * the aescrypt library.
 *
 * Backend selection itself is intentionally not implemented here.
 * This header should expose only the API required by library users.
 */

#ifndef AESCRYPT_PUBLIC_AES_H
#define AESCRYPT_PUBLIC_AES_H

#include <stddef.h>
#include <stdint.h>

typedef struct aescrypt_key aescrypt_key_t;

/**********************************************************
 * Library Constants
 **********************************************************/

/* AES-128 uses a 128-bit(16 Bytes) key. */
#define AESCRYPT_AES128_KEY_SIZE 16U

/*
 * AES operates on 128-bit blocks.
 *
 * CTR mode uses the AES block size for the counter block.
 */
#define AESCRYPT_AES_BLOCK_SIZE 16U

/*
 * CTR block layout:
 *
 *     [ 8-byte nonce ][ 8-byte counter ]
 *
 * The nonce is generated using the CSPRNG.
 * The counter is initialized for each encryption operation
 * and incremented for every AES block processed.
 */
#define AESCRYPT_NONCE_SIZE 8U
#define AESCRYPT_COUNTER_SIZE 8U

/**********************************************************
 * Basic Byte Buffer
 **********************************************************/

/*
 * Generic immutable byte-buffer view.
 *
 * This structure does NOT own the referenced memory.
 *
 * Implementation guidance:
 * - `data` points to the first byte.
 * - `size` contains the number of valid bytes.
 * - The caller remains responsible for the lifetime of `data`.
 * - Do not modify the memory through this type.
 */
typedef struct
{
    const uint8_t *data;
    size_t size;
} aescrypt_bytes_t;

/*
 * Generic mutable byte-buffer.
 *
 * This structure also does NOT own the referenced memory.
 *
 * Implementation guidance:
 * - `data` points to writable memory.
 * - `size` is the number of bytes available.
 * - The caller must provide enough storage for the operation.
 * - Keep ownership rules explicit throughout the API.
 */
typedef struct
{
    uint8_t *data;
    size_t size;
} aescrypt_mutable_bytes_t;


/**********************************************************
 * AES-CTR Nonce / Initial Counter
 **********************************************************/

/*
 * CTR nonce/IV representation.
 *
 * The nonce generation policy is implemented elsewhere.
 *
 * DESIGN REQUIREMENT:
 * The nonce must be generated using a cryptographically secure
 * random number generator.
 *
 * The chosen nonce strategy must be found in DESIGN.md.
 */
typedef struct
{
    uint8_t bytes[AESCRYPT_NONCE_SIZE];
} aescrypt_nonce_t;

/**********************************************************
 * AES-128-CTR Context
 **********************************************************/

/*
 * AES-CTR context.
 *
 * The internal representation should NOT be exposed through
 * the public header unless necessary.
 *
 * If the implementation requires expanded AES round keys,
 * backend state, or other internal data, consider keeping
 * those details in a private implementation structure.
 *
 * The public API should expose only what callers need.
 */
typedef struct aescrypt_aes128_ctr aescrypt_aes128_ctr_t;

/*
 * 64-bit counter component of the CTR block.
 *
 * The complete AES input block is:
 *
 *     [ 8-byte nonce ][ 8-byte counter ]
 *
 * The counter is maintained independently from the nonce.
 */
typedef uint64_t aescrypt_counter_t;

/**********************************************************
 * Context Lifecycle
 **********************************************************/

/*
 * Initialize an AES-128-CTR context.
 *
 * Implementation guidance:
 * - Validate all input pointers.
 * - Validate the key and nonce inputs.
 * - Initialize the AES key schedule required by the selected
 *   backend.
 * - Do not perform CPU feature detection in multiple unrelated
 *   places. Backend selection should have one clear owner.
 * - The normal caller should not need to manually select AES-NI
 *   versus software AES.
 */
int aescrypt_aes128_ctr_init(
    aescrypt_aes128_ctr_t *ctx,
    const aescrypt_key_t *key,
    const aescrypt_nonce_t *nonce);

/*
 * Destroy an AES-128-CTR context.
 *
 * Implementation guidance:
 * - Release all resources owned by the context.
 * - Clear sensitive key-derived material before releasing it.
 * - Be aware that ordinary memset() may be optimized away when
 *   used for secure memory clearing.
 * - The exact secure-clearing mechanism should be selected in
 *   the implementation and documented where appropriate.
 */
void aescrypt_aes128_ctr_destroy(
    aescrypt_aes128_ctr_t *ctx);

/**********************************************************
 * AES-128-CTR Encryption
 **********************************************************/

/*
 * Encrypt plaintext using AES-128-CTR.
 *
 * Implementation guidance:
 * - CTR encryption processes the plaintext using AES-generated
 *   counter blocks.
 * - Encryption and decryption use the same CTR operation.
 * - Do not modify the input buffer.
 * - The output buffer must be large enough for `input->size`.
 * - Handle zero-length input correctly.
 * - Define and document whether the context maintains counter
 *   state between calls or whether each operation represents a
 *   complete independent message.
 * - Ensure counter handling cannot silently wrap into reused
 *   keystream for a single nonce.
 */
int aescrypt_aes128_ctr_encrypt(
    aescrypt_aes128_ctr_t *ctx,
    const aescrypt_bytes_t *input,
    aescrypt_mutable_bytes_t *output);

/**********************************************************
 * AES-128-CTR Decryption
 **********************************************************/

/*
 * Decrypt ciphertext using AES-128-CTR.
 *
 * Implementation guidance:
 * - CTR decryption is the same XOR-with-keystream operation
 *   used for encryption.
 * - Do not implement a separate AES "decrypt mode" for CTR.
 * - The nonce/counter state must correspond to the ciphertext
 *   being decrypted.
 * - Do not modify the ciphertext input buffer.
 * - The output size must match the input size.
 */
int aescrypt_aes128_ctr_decrypt(
    aescrypt_aes128_ctr_t *ctx,
    const aescrypt_bytes_t *input,
    aescrypt_mutable_bytes_t *output);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_PUBLIC_AES_H */