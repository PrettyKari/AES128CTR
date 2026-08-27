/**
 * @file random.c
 * @brief Cryptographically secure random generation wrapper.
 *
 * This file provides the crypto-layer random interface.
 *
 * Platform-specific random generation is hidden behind the
 * platform abstraction layer.
 */


#include "random.h"

#include "../platform/platform.h"


/**********************************************************
 * Random Byte Generation
 **********************************************************/

int aescrypt_random_bytes(
    uint8_t *buffer,
    size_t size)
{
    /*
     * Validate arguments.
     *
     * A NULL buffer is invalid when bytes are requested.
	 */
	if((buffer == NULL) && (size != 0U)){
		return -1;
	}
	
    /*
     * Zero-length requests are already successful.
     *
     * No platform call is necessary.
     */
    if (size == 0U)
    {
        return 0;
    }
	
	return aescrypt_platform_random_bytes(
        buffer,
        size);
    return 0;
}


/**********************************************************
 * End of File
 **********************************************************/