/**
 * @file windows_cpu.h
 * @brief Windows CPU feature detection interface.
 *
 * This header defines the Windows-specific CPU detection interface
 * used by the platform CPU abstraction.
 *
 * The implementation is responsible for determining CPU features
 * at runtime and must not rely solely on compile-time feature
 * detection.
 */

#ifndef AESCRYPT_WINDOWS_CPU_H
#define AESCRYPT_WINDOWS_CPU_H

/**********************************************************
 * Windows CPU Feature Detection
 **********************************************************/

/*
 * Determine whether the current Windows CPU supports AES-NI.
 *
 * CPU detection:
 *
 * - On x86/x86-64, query the processor's CPUID feature information.
 * - Determine the AES feature bit from the appropriate CPUID leaf
 *   and register.
 * - Use the CPU/compiler mechanism appropriate for the supported
 *   Windows toolchain.
 *
 * Toolchain target:
 *
 * - Primary target: MinGW-w64/GCC.
 * - If MSVC support is added later, keep MSVC-specific CPU detection
 *   inside the Windows platform implementation.
 * - Do not expose compiler-specific CPUID types or intrinsics through
 *   this header.
 *
 * Architecture handling:
 *
 * - x86-64 / amd64 must perform the AES-NI feature check.
 * - Non-x86 architectures must safely report that AES-NI is
 *   unavailable unless a future implementation explicitly adds
 *   support for that architecture.
 *
 * Return:
 *     non-zero if AES-NI is supported.
 *     zero otherwise.
 */
int aescrypt_windows_cpu_has_aesni(void);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_WINDOWS_CPU_H */