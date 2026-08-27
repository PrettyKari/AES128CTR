/**
 * @file key.c
 * @brief AES-128 key management implementation.
 *
 * This file implements AES key generation and lifetime handling.
 *
 * It does not perform AES operations or key expansion.
 */


#include "key.h"

#include "random.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>


/**********************************************************
 * Internal Secure Memory Clearing
 **********************************************************/
static void aescrypt_secure_zero(
	void *buffer,
	size_t size)
	{
	volatile uint8_t *ptr;
	
    /*
     * Convert pointer to volatile memory access.
     *
     * The volatile qualifier prevents the compiler from removing
     * these writes as "unnecessary optimization".
     */
	ptr = (volatile uint8_t *)buffer;
	while(size > 0U){
		*ptr = 0U;
		ptr++;
		size--;
	} 
}


/**********************************************************
 * Key Generation
 **********************************************************/

int aescrypt_key_generate(
    aescrypt_key_t *key)
{
	int result;

    /*
     * Fill key material using the crypto random abstraction.
     *
     * Flow:
     *
     *     key.c
     *       |
     *       v
     *     random.c
     *       |
     *       v
     *     platform.c
     *       |
     *       +---- Linux getrandom()
     *       |
     *       +---- Windows BCryptGenRandom()
     */
	 result = aescrypt_random_bytes(
		key->bytes,
		AESCRYPT_AES128_KEY_SIZE);
	if (result != 0){
		
		/* Do not leave partially generated key material. */
		aescrypt_secure_zero(
			key->bytes,
			AESCRYPT_AES128_KEY_SIZE);
		return -1;
	}
	
    return 0;
}



/**********************************************************
 * Secure Key Clearing
 **********************************************************/

void aescrypt_key_clear(
    aescrypt_key_t *key)
{
	
	/* Securely erase key bytes. */
	aescrypt_secure_zero(
		key->bytes,
		AESCRYPT_AES128_KEY_SIZE);
}


/**********************************************************
 * Key Copy
 **********************************************************/

int aescrypt_key_copy(
    aescrypt_key_t *destination,
    const aescrypt_key_t *source)
{
	/*
     * Copy byte-by-byte instead of relying on hidden behavior.
     *
     * This keeps the operation explicit and easy to audit.
     */
    for (size_t index = 0U;
         index < AESCRYPT_AES128_KEY_SIZE;
         index++)
    {
        destination->bytes[index] = source->bytes[index];
    }
	
    return 0;
}



/**********************************************************
 * End of File
 **********************************************************/