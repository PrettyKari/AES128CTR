/**
 * @file aesni.c
 * @brief AES-NI accelerated AES-128 implementation.
 *
 * This file implements AES-128 encryption using Intel AES-NI
 * hardware instructions.
 *
 * This backend does not:
 *
 *      - detect CPU features
 *      - select itself
 *      - implement CTR mode
 *      - generate AES keys
 */


#include "aesni.h"


/**********************************************************
 * Compiler / Platform Includes
 **********************************************************/

/*
 * TODO:
 *
 * Include AES-NI intrinsic header.
 *
 *
 * GCC / Clang:
 *
 *      <wmmintrin.h>
 *
 *
 * MSVC:
 *
 *      <wmmintrin.h>
 *
 *
 * Keep compiler-specific details inside this file.
 */

#include <wmmintrin.h>

/**********************************************************
 * AES-NI Block Encryption
 **********************************************************/

void aescrypt_aesni_encrypt_block(
    const uint8_t input[16],
    uint8_t output[16],
    const uint8_t round_keys[176])
{
    __m128i state;


    /*
     * Load plaintext block.
     *
     * _mm_loadu_si128()
     *
     * is used because the input buffer does not require
     * 16-byte alignment.
     */
    state = _mm_loadu_si128(
        (const __m128i *)input);


    /*
     * Initial AddRoundKey.
     *
     * AES starts with:
     *
     *      state XOR round_key_0
     */
    state = _mm_xor_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[0U]));
	
	/*
     * AES-128 rounds 1-9.
     *
     * AESENC performs:
     *
     *      SubBytes
     *      ShiftRows
     *      MixColumns
     *      AddRoundKey
     */
    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[16U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[32U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[48U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[64U]));
			
	state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[80U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[96U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[112U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[128U]));


    state = _mm_aesenc_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[144U]));
			
	/*
     * Final AES round.
     *
     * AESENCLAST does NOT perform MixColumns.
     */
    state = _mm_aesenclast_si128(
        state,
        _mm_loadu_si128(
            (const __m128i *)&round_keys[160U]));
			
	/*
     * Store ciphertext.
     */
    _mm_storeu_si128(
        (__m128i *)output,
        state);
}



/**********************************************************
 * End of File
 **********************************************************/