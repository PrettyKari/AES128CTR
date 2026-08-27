/**
 * @file dispatch.c
 * @brief AES backend dispatch implementation.
 *
 * This file selects the AES implementation used by the library.
 *
 * It does not contain AES algorithms.
 */


#include "dispatch.h"

#include "detect.h"

#include "../aes/software.h"
#include "../aes/aesni.h"



/**********************************************************
 * Private State
 **********************************************************/

/*
 * Currently selected AES encryption function.
 *
 * Default:
 *
 *      software AES
 *
 * because it exists on every supported platform.
 */
static aescrypt_encrypt_block_fn
    selected_encrypt_function = 0U;



/*
 * Dispatcher initialization state.
 */
static int backend_initialized = 0;



/**********************************************************
 * Backend Initialization
 **********************************************************/

void aescrypt_backend_initialize(void)
{
    /* Avoid repeated initialization. */
    if (backend_initialized != 0)
    {
        return;
    }

    /*
     * Always provide software fallback first.
     *
     * This guarantees that the library has a usable backend.
     */
    selected_encrypt_function =
        aescrypt_software_encrypt_block;
    
    /*
     * Replace software backend with AES-NI if available.
     */
    if (aescrypt_cpu_has_aesni() != 0)
    {
        selected_encrypt_function =
            aescrypt_aesni_encrypt_block;
    }



    backend_initialized = 1;
}



/**********************************************************
 * Backend Function Access
 **********************************************************/

aescrypt_encrypt_block_fn
aescrypt_backend_get_encrypt_function(void)
{

    /*
     * Initialize automatically if needed.
     */
    if (backend_initialized == 0)
    {
        aescrypt_backend_initialize();
    }


    return selected_encrypt_function;
}



/**********************************************************
 * Backend Status
 **********************************************************/

int aescrypt_backend_is_initialized(void)
{
    return backend_initialized;
}



/**********************************************************
 * End of File
 **********************************************************/