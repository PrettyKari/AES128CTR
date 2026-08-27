/**
 * @file file.h
 * @brief File IO abstraction interface.
 *
 * This header defines the internal file handling API used by
 * the container layer.
 *
 * Platform-specific file operations are hidden behind this API.
 */


#ifndef AESCRYPT_IO_FILE_H
#define AESCRYPT_IO_FILE_H


#include <stdint.h>
#include <stddef.h>



/**********************************************************
 * File Handle
 **********************************************************/

/*
 * Internal file object.
 *
 *      FILE *
 *
 *      POSIX file descriptor
 *
 *      Windows HANDLE
 *
 * - Keep ownership clear:
 *
 *      open()
 *          creates resource
 *
 *      close()
 *          releases resource
 */
typedef struct
{
    void *handle;

} aescrypt_file_t;



/**********************************************************
 * File Opening
 **********************************************************/

/*
 * Open file for reading.
 *
 * Return:
 *
 *      0
 *          Success.
 *
 *      non-zero
 *          Failure.
 */
int aescrypt_file_open_read(
    aescrypt_file_t *file,
    const char *path);



/*
 * Open file for writing.
 *
 * Existing file may be replaced.
 */
int aescrypt_file_open_write(
    aescrypt_file_t *file,
    const char *path);



/**********************************************************
 * File Operations
 **********************************************************/

/*
 * Read bytes from file.
 *
 * Return:
 *
 *      number of bytes read
 *
 *      0 on EOF/error
 */
size_t aescrypt_file_read(
    aescrypt_file_t *file,
    uint8_t *buffer,
    size_t size);



/*
 * Write bytes to file.
 *
 * Return:
 *
 *      0
 *          Success.
 *
 *      non-zero
 *          Failure.
 */
int aescrypt_file_write(
    aescrypt_file_t *file,
    const uint8_t *buffer,
    size_t size);



/*
 * Obtain file size.
 */
uint64_t aescrypt_file_size(
    aescrypt_file_t *file);



/**********************************************************
 * File Closing
 **********************************************************/

/*
 * Close file and release resources.
 */
void aescrypt_file_close(
    aescrypt_file_t *file);



/**********************************************************
 * End of File
 **********************************************************/

#endif /* AESCRYPT_IO_FILE_H */