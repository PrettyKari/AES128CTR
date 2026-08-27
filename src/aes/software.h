/**
 * @file software.h
 * @brief Software AES-128 implementation interface.
 *
 * This header defines the internal software AES backend.
 *
 * This implementation provides AES-128 block encryption without
 * hardware acceleration.
 */

#ifndef AESCRYPT_AES_SOFTWARE_H
#define AESCRYPT_AES_SOFTWARE_H


#include <stdint.h>


/**********************************************************
 * AES-128 Software Backend
 **********************************************************/

/*
 * Encrypt one AES-128 block using the software implementation.
 *
 * AES block size:
 *
 *      16 bytes
 *
 *
 * Parameters:
 *
 *      input
 *          Plaintext AES block.
 *
 *      output
 *          Destination ciphertext block.
 *
 *      round_keys
 *          Expanded AES-128 round keys.
 *
 */
void aescrypt_software_encrypt_block(
    const uint8_t input[16],
    uint8_t output[16],
    const uint8_t round_keys[176]);

uint8_t aescrypt_aes_sbox_lookup(
    uint8_t value);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_AES_SOFTWARE_H */