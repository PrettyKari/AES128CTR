/**
 * @file detect.c
 * @brief Runtime CPU feature detection for AES backend selection.
 *
 * This file provides the backend-layer interface for determining
 * whether the current CPU supports AES-NI.
 *
 * Operating-system and compiler-specific CPU detection is delegated
 * to the platform layer.
 */

#include "detect.h"

#include "../platform/platform.h"

/**********************************************************
 * AES-NI Detection
 **********************************************************/

int aescrypt_cpu_has_aesni(void)
{
    /*
     * The backend layer should not know how CPU feature detection
     * is implemented on a particular operating system or compiler.
     *
     * Delegate the actual runtime CPU query to the platform layer.
     *
     * The platform implementation is responsible for:
     *
     *     1. Determining the target architecture.
     *     2. Performing runtime CPU feature detection.
     *     3. Determining whether AES-NI is supported.
     *     4. Returning zero when AES-NI is unavailable.
     *
     * This keeps detect.c completely independent of:
     *
     *     - CPUID intrinsics
     *     - compiler-specific interfaces
     *     - operating-system-specific APIs
     *     - x86-specific implementation details
     */
    return aescrypt_platform_cpu_has_aesni();
}

/**********************************************************
 * End of File
 **********************************************************/