/**
 * @file writer.c
 * @brief Encrypted container writer implementation.
 *
 * Creates AES-CTR encrypted container files.
 */


#include "writer.h"

#include "format.h"

#include "../aes/ctr.h"

#include "../crypto/random.h"

#include "../io/file.h"

#include "aescrypt/key.h"



/**********************************************************
 * Private Helpers
 **********************************************************/

/*
 * Internal helper functions should handle:
 *
 * - chunk processing
 * - temporary buffer management
 * - encryption loop
 *
 * Keep file format logic separate from this layer.
 */


static int writer_generate_nonce(
    uint8_t nonce[8U])
{
    /*
     * Generate cryptographically secure nonce.
     *
     * Nonce uniqueness is critical:
     *
     * Same key + same nonce
     *
     * causes CTR keystream reuse.
     */
    return aescrypt_random_bytes(
        nonce,
        8U);
}

/**********************************************************
 * Container Writer
 **********************************************************/

int aescrypt_container_write(
    const char *input,
    const char *output,
    const aescrypt_key_t *key)
{

    aescrypt_file_t input_file;

    aescrypt_file_t output_file;

    aescrypt_container_header_t header;

    aescrypt_ctr_ctx_t ctr;


    uint8_t nonce[8U];

    uint8_t header_buffer[
        AESCRYPT_CONTAINER_HEADER_SIZE];

    uint8_t input_buffer[
        AESCRYPT_WRITER_BUFFER_SIZE];

    uint8_t output_buffer[
        AESCRYPT_WRITER_BUFFER_SIZE];

    size_t bytes_read;

    uint64_t plaintext_size;

    /*
     * Open plaintext input.
     */
    if (aescrypt_file_open_read(
            &input_file,
            input) != 0)
    {
        return -1;
    }

    /*
     * Open encrypted output.
     */
    if (aescrypt_file_open_write(
            &output_file,
            output) != 0)
    {
        aescrypt_file_close(
            &input_file);

        return -1;
    }
	
    /*
     * Obtain original file size.
     *
     * This value is stored inside
     * the container header.
     */
    plaintext_size =
        aescrypt_file_size(
            &input_file);

    /*
     * Generate unique CTR nonce.
     */
    if (writer_generate_nonce(nonce) != 0)
    {
        aescrypt_file_close(
            &input_file);

        aescrypt_file_close(
            &output_file);

        return -1;
    }	
	
	/*
     * Build container metadata.
     */
    aescrypt_container_header_init(
        &header,
        nonce,
        plaintext_size);

    /*
     * Serialize header.
     */
    aescrypt_container_header_encode(
        &header,
        header_buffer);

    /*
     * Write header first.
     *
     * Container layout:
     *
     *      HEADER
     *      |
     *      v
     *      CIPHERTEXT
     */	
    if (aescrypt_file_write(
            &output_file,
            header_buffer,
            AESCRYPT_CONTAINER_HEADER_SIZE) != 0)
    {
        aescrypt_file_close(
            &input_file);

        aescrypt_file_close(
            &output_file);

        return -1;
    }

    /*
     * Initialize AES-CTR.
     *
     * Counter starts from zero.
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
     * Encrypt file in chunks.
     */
    while (1)
    {
        bytes_read =
            aescrypt_file_read(
                &input_file,
                input_buffer,
                AESCRYPT_WRITER_BUFFER_SIZE);

        /*
         * End of file.
         */
        if (bytes_read == 0U)
        {
            break;
        }

        /*
         * AES-CTR encryption.
         *
         * Same function is used for
         * encryption and decryption.
         */
        aescrypt_ctr_crypt(
            &ctr,
            input_buffer,
            output_buffer,
            bytes_read);

        /*
         * Write encrypted chunk.
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
    }
	

    /*
     * Remove AES state from memory.
     */
    aescrypt_ctr_destroy(
        &ctr);

    /*
     * Close resources.
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