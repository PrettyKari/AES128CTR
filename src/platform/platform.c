/**
 * @file platform.c
 * @brief Platform abstraction implementation.
 *
 * This file provides the common platform API used by the rest of
 * the library.
 *
 * Operating-system specific implementations are selected here.
 *
 * Supported platforms:
 *
 *     - Linux
 *     - Windows
 */


/**********************************************************
 * Platform Selection
 **********************************************************/

/*
 * This file is the ONLY common location where operating-system
 * selection occurs.
 *
 * Higher layers must not contain:
 *
 *     #ifdef _WIN32
 *     #ifdef __linux__
 *
 * because platform knowledge should remain isolated here.
 */

#if defined(_WIN32)

#include "windows/windows_cpu.h"
#include "windows/windows_random.h"


#elif defined(__linux__)

#include "linux/linux_cpu.h"
#include "linux/random_linux.h"


#else

#error "Unsupported operating system"

#endif



#include "platform.h"



/**********************************************************
 * CPU Feature Detection
 **********************************************************/

int aescrypt_platform_cpu_has_aesni(void){


#if defined(_WIN32)

	/*
     * Windows implementation:
     *
     *     windows_cpu.c
     *
     * The platform layer hides the implementation details from
     * the backend.
     */
    return aescrypt_windows_cpu_has_aesni();


#elif defined(__linux__)

    /*
     * Linux implementation:
     *
     *     linux_cpu.c
     */
    return aescrypt_linux_cpu_has_aesni();


#endif
}



/**********************************************************
 * Cryptographically Secure Random Generation
 **********************************************************/

int aescrypt_platform_random_bytes(
    uint8_t *buffer,
    size_t size)
{


#if defined(_WIN32)

    /*
     * Windows CSPRNG implementation:
     *
     *     windows_random.c
     *
     * Internally uses the Windows cryptographic random provider.
     */
    return aescrypt_windows_random_bytes(
        buffer,
        size);


#elif defined(__linux__)

    /*
     * Linux CSPRNG implementation:
     *
     *     linux_random.c
     *
     * Internally uses the Linux kernel random subsystem.
     */
    return aescrypt_linux_random_bytes(
        buffer,
        size);

#endif
}



/**********************************************************
 * End of File
 **********************************************************/