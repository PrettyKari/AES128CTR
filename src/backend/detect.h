/**
 * @file detect.h
 * @brief Runtime CPU feature detection interface.
 *
 * This header defines the internal backend-layer interface used
 * to query CPU features required for AES backend selection.
 *
 * Platform-specific CPU detection is deliberately hidden behind
 * this interface.
 */

#ifndef AESCRYPT_BACKEND_DETECT_H
#define AESCRYPT_BACKEND_DETECT_H

/**********************************************************
 * CPU Feature Detection
 **********************************************************/

/*
 * Determine whether the current CPU supports AES-NI.
 *
 * Implementation:
 *
 * - Perform runtime CPU feature detection.
 * - Do not rely on compile-time AES feature macros.
 * - Do not assume that compiling with AES support means the
 *   executing CPU supports AES-NI.
 * - The platform-specific detection mechanism is hidden from
 *   this interface.
 *
 * Return:
 *     non-zero if AES-NI is supported.
 *     zero otherwise.
 */
int aescrypt_cpu_has_aesni(void);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_BACKEND_DETECT_H */