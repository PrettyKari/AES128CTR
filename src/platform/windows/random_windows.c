/**
 * @file windows_random.c
 * @brief Windows cryptographically secure random implementation.
 *
 * This file provides the Windows-specific implementation of the
 * platform random-byte interface.
 *
 * The random source must be provided by the operating system's
 * cryptographic random facility.
 */

#include "random_windows.h"

#include <windows.h>
#include <bcrypt.h>

/**********************************************************
 * Windows CSPRNG
 **********************************************************/

int aescrypt_windows_random_bytes(
    uint8_t *buffer,
    size_t size)
	{
	NTSTATUS status;
	
	
    /*
     * Validate input arguments.
     *
     * A NULL buffer is only acceptable when no bytes are requested.
     */
	if ((buffer == NULL) && (size != 0U)){
		 return -1;
	}
	
	 /*
     * A zero-length random request is already satisfied.
     *
     * No operating system call is required.
     */
	if (size == 0U){
		return 0;
	}
	
    /*
     * Request cryptographically secure random bytes from Windows.
     *
     * BCryptGenRandom():
     *
     *     - Uses the Windows CNG random provider.
     *     - Is suitable for cryptographic key and nonce generation.
     *     - Does not require manual seeding.
     *
     * BCRYPT_USE_SYSTEM_PREFERRED_RNG:
     *
     *     Use the system-selected cryptographic random provider.
     */
	
	status = BCryptGenRandom(
		NULL,
		buffer,
		(ULONG)size,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	);
	
	/* STATUS_SUCCESS indicates successful random generation. */
    if (status != STATUS_SUCCESS){
		/* Random generation failure. */
		return -1;
	}
	
	return 0;
}

/**********************************************************
 * End of File
 **********************************************************/