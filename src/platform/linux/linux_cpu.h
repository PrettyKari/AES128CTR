/**
 * @file linux_cpu.h
 * @brief Linux CPU feature detection interface.
 *
 * This header defines the Linux-specific CPU detection interface
 * used by the platform CPU abstraction.
 *
 * Runtime CPU feature detection is performed by linux_cpu.c.
 */

#ifndef AESCRYPT_LINUX_CPU_H
#define AESCRYPT_LINUX_CPU_H

/**********************************************************
 * Linux CPU Feature Detection
 **********************************************************/

/*
 * Determine whether the current Linux CPU supports AES-NI.
 *
 * CPU detection:
 *
 * - On x86/x86-64, query the processor's CPUID feature information.
 * - Determine the AES-NI feature from the appropriate CPUID
 *   feature register.
 *
 * Toolchain target:
 *
 * - Primary target: GCC on Linux.
 * - Keep compiler-specific CPU detection details inside the
 *   implementation file.
 * - Do not expose CPUID-specific types, registers, or compiler
 *   intrinsics through this header.
 *
 * Architecture handling:
 *
 * - x86-64 / amd64 must perform the AES-NI feature check.
 * - x86 may also perform the same runtime check if supported by
 *   the implementation.
 * - Unsupported architectures must safely report that AES-NI
 *   is unavailable.
 *
 * Return:
 *     non-zero if AES-NI is supported.
 *     zero otherwise.
 */
int aescrypt_linux_cpu_has_aesni(void);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_LINUX_CPU_H */