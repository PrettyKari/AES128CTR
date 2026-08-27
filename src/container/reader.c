/**
 * @file reader.c
 * @brief Encrypted container reader implementation.
 */


#include "reader.h"

#include "format.h"

#include "../aes/ctr.h"

#include "../io/file.h"



/**********************************************************
 * Container Reader
 **********************************************************/

int aescrypt_container_read(
    const char *input_path,
    const char *output_path,
    const aescrypt_key_t *key)
{
    aescrypt_file_t input_file;

    aescrypt_file_t output_file;


    aescrypt_container_header_t header;


    aescrypt_ctr_ctx_t ctr;


    uint8_t header_buffer[
        AESCRYPT_CONTAINER_HEADER_SIZE];


    uint8_t input_buffer[
        AESCRYPT_READER_BUFFER_SIZE];


    uint8_t output_buffer[
        AESCRYPT_READER_BUFFER_SIZE];


    uint8_t nonce[8U];


    size_t bytes_read;


    uint64_t remaining_size;


    uint8_t index;



    /*
     * Open encrypted container.
     */
    if (aescrypt_file_open_read(
            &input_file,
            input_path) != 0)
    {
        return -1;
    }



    /*
     * Open plaintext output.
     */
    if (aescrypt_file_open_write(
            &output_file,
            output_path) != 0)
    {
        aescrypt_file_close(
            &input_file);

        return -1;
    }



    /*
     * Read container header.
     */
    if (aescrypt_file_read(
            &input_file,
            header_buffer,
            AESCRYPT_CONTAINER_HEADER_SIZE)
        != AESCRYPT_CONTAINER_HEADER_SIZE)
    {
        aescrypt_file_close(
            &input_file);

        aescrypt_file_close(
            &output_file);

        return -1;
    }



    /*
     * Decode header.
     *
     * This validates:
     *
     * - magic
     * - version
     *
     */
    if (aescrypt_container_header_decode(
            &header,
            header_buffer) != 0)
    {
        aescrypt_file_close(
            &input_file);

        aescrypt_file_close(
            &output_file);

        return -1;
    }



    /*
     * Copy nonce from header.
     */
    for (index = 0U;
         index < AESCRYPT_NONCE_SIZE;
         index++)
    {
        nonce[index] =
            header.nonce[index];
    }



    /*
     * Initialize AES-CTR.
     *
     * Same nonce + same key
     * recreates the same keystream.
     */
    if (aescrypt_ctr_init(
            &ctr,
            key->bytes,
            nonce) != 0)
    {
        aescrypt_file_close(
            &input_file);

        aescrypt_file_close(
            &output_file);

        return -1;
    }



    /*
     * Expected plaintext size.
     *
     * Prevent writing extra data if the
     * container contains trailing bytes.
     */
    remaining_size =
        header.plaintext_size;



    /*
     * Decrypt ciphertext chunks.
     */
    while (remaining_size > 0U)
    {
        size_t chunk_size;



        chunk_size =
            AESCRYPT_READER_BUFFER_SIZE;



        if (remaining_size < chunk_size)
        {
            chunk_size =
                (size_t)remaining_size;
        }



        /*
         * Read ciphertext chunk.
         */
        bytes_read =
            aescrypt_file_read(
                &input_file,
                input_buffer,
                chunk_size);



        if (bytes_read != chunk_size)
        {
            aescrypt_ctr_destroy(
                &ctr);

            aescrypt_file_close(
                &input_file);

            aescrypt_file_close(
                &output_file);

            return -1;
        }



        /*
         * AES-CTR decrypt.
         *
         * CTR encryption and decryption
         * are identical operations.
         */
        aescrypt_ctr_crypt(
            &ctr,
            input_buffer,
            output_buffer,
            bytes_read);



        /*
         * Write plaintext.
         */
        if (aescrypt_file_write(
                &output_file,
                output_buffer,
                bytes_read) != 0)
        {
            aescrypt_ctr_destroy(
                &ctr);

            aescrypt_file_close(
                &input_file);

            aescrypt_file_close(
                &output_file);

            return -1;
        }



        remaining_size -= bytes_read;
    }



    /*
     * Remove sensitive AES state.
     */
    aescrypt_ctr_destroy(
        &ctr);



    /*
     * Close files.
     */
    aescrypt_file_close(
        &input_file);


    aescrypt_file_close(
        &output_file);



    return 0;
}



/**********************************************************
 * End of File
 **********************************************************/