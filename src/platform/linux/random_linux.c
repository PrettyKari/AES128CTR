/**
 * @file linux_random.c
 * @brief Linux cryptographically secure random implementation.
 *
 * This file implements the Linux-specific random byte provider.
 *
 * The implementation details of Linux random generation remain
 * private to this module.
 */

#include "./random_linux.h"

#include <errno.h>
#include <sys/random.h>

/**********************************************************
 * Linux CSPRNG Interface
 **********************************************************/

int aescrypt_linux_random_bytes(
    uint8_t *buffer,
    size_t size)
{

	size_t offset = 0U;

    /*
     * Validate input arguments.
     *
     * A NULL buffer is invalid when random bytes are requested.
     *
     * Example:
     *
     *     buffer = NULL
     *     size   = 16
     *
     * would result in an invalid memory write.
     */
	if ((buffer == NULL) && (size != 0U)){
		return -1;
	}

    /*
     * A zero-byte request is already satisfied.
     *
     * No kernel call is required.
	 */
	if (size == 0U){
		return 0;
	}

    /*
     * Generate random bytes using the Linux kernel CSPRNG.
     *
     * getrandom() may return fewer bytes than requested.
     *
     * Therefore:
     *
     *     requested:
     *         32 bytes
     *
     *     possible first result:
     *         16 bytes
     *
     *     remaining:
     *         16 bytes
     *
     * Continue until the complete buffer has been filled.
     */
	while (offset < size){
		ssize_t result;
		
		result = getrandom(
		buffer + offset,
		size - offset,
		0);
        /*
         * getrandom() failed.
         */
		if (result < 0){
            /*
			 * The syscall was interrupted before completion.
             *
             * Retry the operation.
             */
			if (errno == EINTR){
				continue;
			}
            /*
             * Other errors:
             *
             * Examples:
             *
             *     - invalid arguments
             *     - kernel failure
             *     - unsupported environment
             *
             * Do not return partially generated data as success.
             */
			return -1;
		}
		
		offset += (size_t)result;;
		
	/* Entire requested buffer has been filled with random bytes. */		
	}
	return 0;
}

/**********************************************************
 * End of File
 **********************************************************/