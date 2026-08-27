/**
 * @file aesni.h
 * @brief AES-NI accelerated AES-128 implementation interface.
 *
 * This header defines the hardware accelerated AES backend.
 *
 * AES-NI availability is checked separately by the backend
 * detection layer.
 *
 * This file only provides the AES-NI encryption primitive.
 */

#ifndef AESCRYPT_AESNI_H
#define AESCRYPT_AESNI_H


#include <stdint.h>



/**********************************************************
 * AES-NI Backend
 **********************************************************/

/*
 * Encrypt one AES-128 block using Intel AES-NI.
 *
 *
 * Parameters:
 *
 *      input
 *          16-byte plaintext AES block.
 *
 *
 *      output
 *          16-byte ciphertext destination.
 *
 *
 *      round_keys
 *          Expanded AES-128 key schedule.
 *
 */
void aescrypt_aesni_encrypt_block(
    const uint8_t input[16],
    uint8_t output[16],
    const uint8_t round_keys[176]);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_AESNI_H */