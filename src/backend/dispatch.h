/**
 * @file dispatch.h
 * @brief AES backend dispatch interface.
 *
 * This header defines the internal mechanism used to select
 * and access the active AES implementation.
 *
 * Backend implementations:
 *
 *      software AES
 *      AES-NI AES
 *
 * are hidden behind this dispatch layer.
 */

#ifndef AESCRYPT_DISPATCH_H
#define AESCRYPT_DISPATCH_H


#include <stdint.h>


/**********************************************************
 * AES Backend Function Type
 **********************************************************/

/*
 * AES-128 block encryption function pointer type.
 *
 * Every AES backend must provide a function matching this
 * interface.
 *
 *
 * Parameters:
 *
 *      input
 *          16-byte plaintext block.
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
typedef void (*aescrypt_encrypt_block_fn)(
    const uint8_t input[16],
    uint8_t output[16],
    const uint8_t round_keys[176]);



/**********************************************************
 * Backend Initialization
 **********************************************************/

/*
 * Initialize AES backend dispatch.
 *
 *
 * Expected flow:
 *
 *
 * aescrypt_backend_initialize()
 *
 *          |
 *          v
 *
 * aescrypt_cpu_has_aesni()
 *
 *          |
 *          v
 *
 * select AES-NI or software
 *
 *
 * This function should normally be called once during
 * library initialization.
 */
void aescrypt_backend_initialize(void);



/**********************************************************
 * Backend Function Access
 **********************************************************/

/*
 * Return the currently selected AES encryption function.
 *
 */
aescrypt_encrypt_block_fn
aescrypt_backend_get_encrypt_function(void);



/**********************************************************
 * Backend Status
 **********************************************************/

/*
 * Return whether the backend dispatcher has been initialized.
 *
 *
 * Return:
 *
 *      non-zero:
 *          dispatcher ready.
 *
 *      zero:
 *          initialization not performed.
 */
int aescrypt_backend_is_initialized(void);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_DISPATCH_H */