/**
 * @file error.h
 * @brief Library-wide error codes and error handling definitions.
 *
 * This header defines the common error representation used by
 * the public aescrypt API.
 *
 * All public operations that can fail should use the same error
 * convention rather than defining unrelated error values in
 * individual modules.
 */

#ifndef AESCRYPT_ERROR_H
#define AESCRYPT_ERROR_H

/**********************************************************
 * Error Codes
 **********************************************************/

/*
 * Library operation result.
 *
 * Implementation guidance:
 *
 * - AESCRYPT_SUCCESS must represent successful completion.
 * - Error values must be stable and unique.
 * - Keep the enumeration independent from operating-system-specific
 *   errno values and platform error codes.
 * - Platform-specific failures should be translated into one of
 *   these library-level errors at the appropriate abstraction layer.
 */
typedef enum
{
    AESCRYPT_SUCCESS = 0,

    /*
     * Generic invalid argument.
     *
     * Use when a caller supplies NULL, an invalid size, an invalid
     * enum value, or another argument that violates the API contract.
     */
    AESCRYPT_ERROR_INVALID_ARGUMENT,

    /*
     * Memory/resource allocation failure.
     *
     * Use when the library cannot obtain required memory or another
     * required runtime resource.
     */
    AESCRYPT_ERROR_OUT_OF_MEMORY,

    /*
     * Cryptographically secure random source failure.
     *
     * Use when the platform CSPRNG cannot provide the requested
     * random data.
     */
    AESCRYPT_ERROR_RANDOM_FAILURE,

    /*
     * CPU/backend capability failure.
     *
     * Use when a requested backend cannot be used on the current
     * platform or CPU.
     */
    AESCRYPT_ERROR_BACKEND_UNAVAILABLE,

    /*
     * Invalid or unusable cryptographic state.
     *
     * Use when an operation is attempted with a context, key,
     * nonce, or other state that is not valid for that operation.
     */
    AESCRYPT_ERROR_INVALID_STATE,

    /*
     * Input data is invalid.
     *
     * Use when supplied data does not satisfy the expected format
     * or structural requirements of the operation.
     */
    AESCRYPT_ERROR_INVALID_DATA,

    /*
     * File or filesystem operation failure.
     *
     * Use for failures while opening, reading, writing, seeking,
     * or closing files.
     */
    AESCRYPT_ERROR_FILE,

    /*
     * Invalid or unsupported encrypted-container format.
     *
     * This is intentionally separate from generic invalid data so
     * the container layer can distinguish malformed/unsupported
     * file structures.
     */
    AESCRYPT_ERROR_INVALID_FORMAT,

    /*
     * Integer/counter overflow condition.
     *
     * Particularly relevant to AES-CTR when the counter can no
     * longer be incremented without wrapping.
     */
    AESCRYPT_ERROR_COUNTER_OVERFLOW,

    /*
     * Generic internal failure.
     *
     * Use only when the failure cannot be represented more
     * specifically by another library error.
     */
    AESCRYPT_ERROR_INTERNAL

} aescrypt_error_t;

/**********************************************************
 * Error Description
 **********************************************************/

/*
 * Convert a library error code to a human-readable description.
 *
 * Implementation guidance:
 *
 * - Return a static string.
 * - The caller must not free the returned pointer.
 * - Every defined error code should have a corresponding
 *   description.
 * - Unknown/invalid enum values should return a generic
 *   "unknown error" description rather than causing undefined
 *   behavior.
 *
 * This function is intended for diagnostics, tests, and the
 * example program.
 */
const char *aescrypt_error_string(
    aescrypt_error_t error);

/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_ERROR_H */