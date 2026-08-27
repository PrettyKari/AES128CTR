/**
 * @file backend.h
 * @brief AES backend selection and identification.
 *
 * This header defines the public representation of the AES backend
 * selected by the library.
 *
 * The library supports two AES execution paths:
 *
 *     - Hardware-accelerated AES-NI
 *     - Software AES fallback
 *
 * Backend availability is determined at runtime.
 * Normal library users should not need to select a backend manually.
 *
 * The selected backend is exposed through this interface so that
 * applications and tests can determine which implementation is active.
 */

#ifndef AESCRYPT_BACKEND_H
#define AESCRYPT_BACKEND_H

#include <stdint.h>

/**********************************************************
 * AES Backend Types
 **********************************************************/

/*
 * AES backend implementations supported by the library.
 *
 * Implementation guidance:
 *
 * AESCRYPT_BACKEND_SOFTWARE
 *     Software AES implementation.
 *
 * AESCRYPT_BACKEND_AESNI
 *     Hardware-accelerated implementation using AES-NI.
 *
 * Keep this enumeration independent from CPU detection.
 * A backend being represented here does NOT mean that the
 * current machine supports it.
 */
typedef enum
{
    AESCRYPT_BACKEND_SOFTWARE = 0,
    AESCRYPT_BACKEND_AESNI

} aescrypt_backend_t;

/**********************************************************
 * Backend Detection
 **********************************************************/

/*
 * Determine whether the current CPU supports AES-NI.
 *
 * Implementation guidance:
 *
 * - This is a CPU capability query, not backend selection.
 * - On amd64, perform the required runtime CPU feature detection.
 * - Do NOT rely on compile-time-only selection.
 * - Do NOT make the result depend on compiler flags such as
 *   -march=native.
 * - The result must describe the CPU on which the binary is
 *   actually running.
 *
 * Platform considerations:
 *
 * - The challenge specifically requires runtime AES-NI detection
 *   on amd64.
 * - Keep architecture-specific detection code out of the public
 *   API implementation where possible.
 * - Unsupported architectures should have a defined result rather
 *   than causing compilation or runtime problems.
 *
 * Return:
 *     non-zero if AES-NI is available.
 *     zero otherwise.
 */
int aescrypt_cpu_has_aesni(void);

/**********************************************************
 * Backend Selection
 **********************************************************/

/*
 * Select the backend automatically.
 *
 * Implementation guidance:
 *
 * - This function represents the library's normal backend policy.
 * - If AES-NI is available, select AESCRYPT_BACKEND_AESNI.
 * - Otherwise select AESCRYPT_BACKEND_SOFTWARE.
 * - Do not require normal callers to make this decision.
 *
 * IMPORTANT:
 * Keep detection and selection conceptually separate:
 *
 *     CPU capability
 *           |
 *           v
 *     backend policy
 *           |
 *           v
 *     selected backend
 */
aescrypt_backend_t aescrypt_backend_select(void);

/**********************************************************
 * Backend Identification
 **********************************************************/

/*
 * Return the backend currently selected by the library.
 *
 * Implementation guidance:
 *
 * - The result must be observable for testing.
 * - The implementation should have one clear source of truth
 *   for the selected backend.
 * - Avoid performing independent CPU detection every time a
 *   caller asks for the backend.
 * - The value returned here should correspond to the backend
 *   actually used by the AES operation.
 */
aescrypt_backend_t aescrypt_backend_current(void);

/**********************************************************
 * Backend Name
 **********************************************************/

/*
 * Return a human-readable name for an AES backend.
 *
 * Expected conceptual values:
 *
 *     "software"
 *     "aes-ni"
 *
 * Implementation guidance:
 *
 * - Return a static string.
 * - The returned string must not require caller-side memory
 *   management.
 * - Keep this function suitable for diagnostic output from
 *   main.c and tests.
 */
const char *aescrypt_backend_name(
    aescrypt_backend_t backend);

/**********************************************************
 * Backend Availability
 **********************************************************/

/*
 * Determine whether a specific backend can be used on the
 * current machine.
 *
 * Implementation guidance:
 *
 * - Software AES should always be considered available when
 *   the library itself supports the current architecture.
 * - AES-NI availability must correspond to actual runtime
 *   CPU support.
 * - Do not report AES-NI as available merely because the code
 *   was compiled with AES-related compiler options.
 *
 * This function is useful for tests and diagnostics.
 */
int aescrypt_backend_is_available(
    aescrypt_backend_t backend);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_BACKEND_H */