/**
 * @file test_backend.c
 * @brief Backend selection test.
 */


#include <stdio.h>


#include "aescrypt/backend.h"



int main(void)
{
    aescrypt_backend_t backend;



    backend =
        aescrypt_backend_select();



    printf(
        "Selected backend: %s\n",
        aescrypt_backend_name(
            backend));



    if (!aescrypt_backend_is_available(
            backend))
    {
        printf(
            "Backend unavailable\n");

        return 1;
    }



    printf(
        "Backend test passed\n");


    return 0;
}