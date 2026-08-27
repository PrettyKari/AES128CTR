/**
 * @file container.c
 * @brief Public container API implementation.
 */


#include "aescrypt/container.h"


#include "writer.h"
#include "reader.h"



/**********************************************************
 * Public Container API
 **********************************************************/

int aescrypt_encrypt_file(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key)
{
    /*
     * Forward request to the internal writer.
     *
     * Public API does not expose the
     * internal container workflow.
     */
    return aescrypt_container_write(
        input_path,
        output_path,
        key);
}



int aescrypt_decrypt_file(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key)
{
    /*
     * Forward request to the internal reader.
     */
    return aescrypt_container_read(
        input_path,
        output_path,
        key);
}



/**********************************************************
 * End of File
 **********************************************************/