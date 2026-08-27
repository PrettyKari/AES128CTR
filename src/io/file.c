/**
 * @file file.c
 * @brief File IO abstraction implementation.
 */


#include "file.h"


#include <stdio.h>



/**********************************************************
 * File Opening
 **********************************************************/

int aescrypt_file_open_read(
    aescrypt_file_t *file,
    const char *path)
{
    FILE *stream;


    stream =
        fopen(
            path,
            "rb");


    if (stream == NULL)
    {
        return -1;
    }


    file->handle =
        stream;


    return 0;
}



int aescrypt_file_open_write(
    aescrypt_file_t *file,
    const char *path)
{
    FILE *stream;


    stream =
        fopen(
            path,
            "wb");


    if (stream == NULL)
    {
        return -1;
    }


    file->handle =
        stream;


    return 0;
}



/**********************************************************
 * File Operations
 **********************************************************/

size_t aescrypt_file_read(
    aescrypt_file_t *file,
    uint8_t *buffer,
    size_t size)
{
    FILE *stream;


    stream =
        (FILE *)file->handle;


    return fread(
        buffer,
        1U,
        size,
        stream);
}



int aescrypt_file_write(
    aescrypt_file_t *file,
    const uint8_t *buffer,
    size_t size)
{
    FILE *stream;

    size_t written;



    stream =
        (FILE *)file->handle;



    written =
        fwrite(
            buffer,
            1U,
            size,
            stream);



    if (written != size)
    {
        return -1;
    }



    return 0;
}



uint64_t aescrypt_file_size(
    aescrypt_file_t *file)
{
    FILE *stream;

    long current;

    long size;



    stream =
        (FILE *)file->handle;



    current =
        ftell(stream);



    fseek(
        stream,
        0L,
        SEEK_END);



    size =
        ftell(stream);



    fseek(
        stream,
        current,
        SEEK_SET);



    return (uint64_t)size;
}



/**********************************************************
 * File Closing
 **********************************************************/

void aescrypt_file_close(
    aescrypt_file_t *file)
{
    FILE *stream;



    if (file->handle == NULL)
    {
        return;
    }



    stream =
        (FILE *)file->handle;



    fclose(stream);



    file->handle =
        NULL;
}



/**********************************************************
 * End of File
 **********************************************************/