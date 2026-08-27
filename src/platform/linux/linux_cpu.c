/**
 * @file linux_cpu.c
 * @brief Linux runtime CPU feature detection.
 *
 * This file implements runtime detection of CPU features required
 * by the AES backend on Linux.
 *
 * The primary Linux toolchain target is GCC.
 */

#include "linux_cpu.h"
#include <cpuid.h>
/**********************************************************
 * Architecture Detection
 **********************************************************/

/*
 * Determine whether the current compilation target is an x86
 * architecture.
 *
 * Expected architecture groups:
 *
 *     x86-64 / amd64
 *         __x86_64__
 *
 *     32-bit x86
 *         __i386__
 *
 * For x86/x86-64:
 *
 *     perform runtime CPUID detection.
 *
 * For unsupported architectures:
 *
 *     report AES-NI as unavailable.
 *
 * IMPORTANT:
 *
 * Architecture macros only describe the compilation target.
 * They must NOT be used as proof that AES-NI exists on the
 * processor running the binary.
 */
#if defined(__x86_64__) || defined(__i386__)

#define AESCRYPT_LINUX_X86 1

#endif
/**********************************************************
 * CPUID Support
 **********************************************************/

/*
 * For the primary GCC/Linux implementation:
 *
 * - Use the GCC-compatible CPU detection facility appropriate
 *   for x86/x86-64.
 *
 * - Keep all compiler-specific CPUID details inside this file.
 *
 * - Do not expose CPUID register representations through the
 *   public or platform headers.
 *
 * The implementation should:
 *
 *     1. Determine whether the required CPUID leaf is available.
 *
 *     2. Query the appropriate CPUID feature leaf.
 *
 *     3. Obtain the feature register containing the AES-NI bit.
 *
 *     4. Test the AES-NI feature bit.
 *
 *     5. Return non-zero when AES-NI is supported.
 *
 *     6. Return zero when AES-NI is unavailable.
 *
 * The implementation must not execute AES instructions merely
 * to determine whether AES-NI is supported.
 */

/**********************************************************
 * AES-NI Detection
 **********************************************************/

int aescrypt_linux_cpu_has_aesni(void)
{
	#if defined(AESCRYPT_LINUX_X86)

    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;

    /*
     * CPUID leaf 1 contains the processor feature information.
     *
     * __get_cpuid() writes:
     *
     *     EAX -> eax
     *     EBX -> ebx
     *     ECX -> ecx
     *     EDX -> edx
     *
     * The function returns non-zero when the requested CPUID
     * information is available.
     *
     * AES-NI is advertised through:
     *
     *     CPUID.01H:ECX[25]
     */
    if (!__get_cpuid(1U, &eax, &ebx, &ecx, &edx))
    {
        /*
         * The required CPUID information is unavailable.
         *
         * Treat AES-NI as unsupported rather than making any
         * assumptions about the processor.
         */
        return 0;
    }

    /*
     * Test CPUID.01H:ECX[25].
     *
     * Bit 25 indicates support for the AES instruction set.
     */
    if ((ecx & (1U << 25)) != 0U)
    {
        return 1;
    }

    /* AES-NI is not advertised by the processor. */
    return 0;

	#else

    /*
     * AES-NI detection is currently implemented only for x86
     * and x86-64.
     *
     * Unsupported architectures must safely report that AES-NI
     * is unavailable so that the software backend can be used.
     */
    return 0;

	#endif
}


/**********************************************************
 * End of File
 **********************************************************/