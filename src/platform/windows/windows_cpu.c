/**
 * @file windows_cpu.c
 * @brief Windows runtime CPU feature detection.
 *
 * This file implements runtime detection of CPU features required
 * by the AES backend on Windows.
 *
 * The current implementation target is x86/x86-64 Windows using
 * MinGW-w64/GCC.
 */

#include "windows_cpu.h"
#incldue <cpuid.h>
/**********************************************************
 * Platform / Architecture Detection
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
#if defined (__x86_64__) || defined(_M_X64)

#define AESCRYPT_WINDOWS_X86_64 1

#elif defined(__i386__) || defined(_M_IX86)

#define AESCRYPT_WINDOWS_X86 1

#endif
/**********************************************************
 * CPUID Support
 **********************************************************/

/*
 * For the primary MinGW-w64/GCC target:
 *
 * - Use the GCC-compatible CPUID facility available to the
 *   selected toolchain.
 *
 * - Verify the exact signature and availability of the facility
 *   rather than assuming that every Windows GCC environment exposes
 *   it identically.
 *
 * - If the selected facility requires an output register array,
 *   keep that representation entirely inside this source file.
 *
 * - Do not expose CPUID implementation details through windows_cpu.h.
 *
 * This implementation should:
 *
 *     1. Determine whether the processor supports the relevant
 *        CPUID leaf.
 *
 *     2. Query the appropriate CPUID feature leaf.
 *
 *     3. Read the feature register containing the AES-NI flag.
 *
 *     4. Test the AES feature bit.
 *
 *     5. Return non-zero when the bit is set.
 *
 *     6. Return zero otherwise.
 *
 * If MSVC support is later required:
 *
 *     - Keep the MSVC-specific intrinsic usage here.
 *     - Do not change the public/backend API.
 *     - Do not leak MSVC-specific types into the common headers.
 */

/**********************************************************
 * AES-NI Detection
 **********************************************************/

int aescrypt_windows_cpu_has_aesni(void)
{
	#if defined(AESCRYPT_WINDOWS_X86_64) || defined(AESCRYPT_WINDOWS_X86)
	 
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	 
	/*
     * AES-NI is advertised through CPUID leaf 1, ECX.
     *
     * Before querying a CPUID leaf, check that the processor
     * supports the leaf.
     *
     * __get_cpuid() performs the required CPUID query and fills
     * the supplied output registers.
     *
     * The function returns non-zero when the requested CPUID
     * information is available.
     */
	if (!__get_cpuid(1U, &eax, &ebx, &ecx, &edx)){
		/* The required CPUID information is unavailable. */
		return 0; 
	}
	/*
	 * AES-NI is represented by CPUID leaf 1, ECX bit 25.
	 *
	 * ECX:
	 *
	 *     bit 25 = AES instruction-set support
	 *
	 * Test only the AES feature bit.
	 */
	if ((ecx & (1U << 25)) != 0U){
		 return 1;
	}
    /* AES-NI is not advertised by the processor. */
    return 0;
	
	#else
    /*
     * AES-NI detection is currently implemented only for x86/x86-64.
     *
     * Unsupported architectures must safely report that AES-NI
     * is unavailable. The software AES backend can subsequently
     * be selected by the backend dispatch layer.
     */
    return 0;

	#endif	
}


/**********************************************************
 * End of File
 **********************************************************/