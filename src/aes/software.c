/**
 * @file software.c
 * @brief Software AES-128 implementation.
 *
 * Portable AES-128 block encryption backend.
 *
 * This file contains:
 *
 *      - AES transformations
 *      - AES round processing
 *      - Software block encryption
 *
 * This file does not contain:
 *
 *      - CTR mode
 *      - key generation
 *      - backend selection
 */


#include "software.h"

#include <stdint.h>


/**********************************************************
 * AES Constants
 **********************************************************/

static const uint8_t aes_sbox[256] =
{
    0x63U, 0x7cU, 0x77U, 0x7bU, 0xf2U, 0x6bU, 0x6fU, 0xc5U,
    0x30U, 0x01U, 0x67U, 0x2bU, 0xfeU, 0xd7U, 0xabU, 0x76U,
    0xcaU, 0x82U, 0xc9U, 0x7dU, 0xfaU, 0x59U, 0x47U, 0xf0U,
    0xadU, 0xd4U, 0xa2U, 0xafU, 0x9cU, 0xa4U, 0x72U, 0xc0U,
    0xb7U, 0xfdU, 0x93U, 0x26U, 0x36U, 0x3fU, 0xf7U, 0xccU,
    0x34U, 0xa5U, 0xe5U, 0xf1U, 0x71U, 0xd8U, 0x31U, 0x15U,
    0x04U, 0xc7U, 0x23U, 0xc3U, 0x18U, 0x96U, 0x05U, 0x9aU,
    0x07U, 0x12U, 0x80U, 0xe2U, 0xebU, 0x27U, 0xb2U, 0x75U,
    0x09U, 0x83U, 0x2cU, 0x1aU, 0x1bU, 0x6eU, 0x5aU, 0xa0U,
    0x52U, 0x3bU, 0xd6U, 0xb3U, 0x29U, 0xe3U, 0x2fU, 0x84U,
    0x53U, 0xd1U, 0x00U, 0xedU, 0x20U, 0xfcU, 0xb1U, 0x5bU,
    0x6aU, 0xcbU, 0xbeU, 0x39U, 0x4aU, 0x4cU, 0x58U, 0xcfU,
    0xd0U, 0xefU, 0xaaU, 0xfbU, 0x43U, 0x4dU, 0x33U, 0x85U,
    0x45U, 0xf9U, 0x02U, 0x7fU, 0x50U, 0x3cU, 0x9fU, 0xa8U,
    0x51U, 0xa3U, 0x40U, 0x8fU, 0x92U, 0x9dU, 0x38U, 0xf5U,
    0xbcU, 0xb6U, 0xdaU, 0x21U, 0x10U, 0xffU, 0xf3U, 0xd2U,
    0xcdU, 0x0cU, 0x13U, 0xecU, 0x5fU, 0x97U, 0x44U, 0x17U,
    0xc4U, 0xa7U, 0x7eU, 0x3dU, 0x64U, 0x5dU, 0x19U, 0x73U,
    0x60U, 0x81U, 0x4fU, 0xdcU, 0x22U, 0x2aU, 0x90U, 0x88U,
    0x46U, 0xeeU, 0xb8U, 0x14U, 0xdeU, 0x5eU, 0x0bU, 0xdbU,
    0xe0U, 0x32U, 0x3aU, 0x0aU, 0x49U, 0x06U, 0x24U, 0x5cU,
    0xc2U, 0xd3U, 0xacU, 0x62U, 0x91U, 0x95U, 0xe4U, 0x79U,
    0xe7U, 0xc8U, 0x37U, 0x6dU, 0x8dU, 0xd5U, 0x4eU, 0xa9U,
    0x6cU, 0x56U, 0xf4U, 0xeaU, 0x65U, 0x7aU, 0xaeU, 0x08U,
    0xbaU, 0x78U, 0x25U, 0x2eU, 0x1cU, 0xa6U, 0xb4U, 0xc6U,
    0xe8U, 0xddU, 0x74U, 0x1fU, 0x4bU, 0xbdU, 0x8bU, 0x8aU,
    0x70U, 0x3eU, 0xb5U, 0x66U, 0x48U, 0x03U, 0xf6U, 0x0eU,
    0x61U, 0x35U, 0x57U, 0xb9U, 0x86U, 0xc1U, 0x1dU, 0x9eU,
    0xe1U, 0xf8U, 0x98U, 0x11U, 0x69U, 0xd9U, 0x8eU, 0x94U,
    0x9bU, 0x1eU, 0x87U, 0xe9U, 0xceU, 0x55U, 0x28U, 0xdfU,
    0x8cU, 0xa1U, 0x89U, 0x0dU, 0xbfU, 0xe6U, 0x42U, 0x68U,
    0x41U, 0x99U, 0x2dU, 0x0fU, 0xb0U, 0x54U, 0xbbU, 0x16U
};



/**********************************************************
 * AES Internal Operations
 **********************************************************/

static uint8_t xtime(uint8_t value){
	return (uint8_t)(value << 1U)^
	((value & 0x80U)
		? 0x1bU
		: 0x00U);
}

static void add_round_key(
	uint8_t state[16],
	const uint8_t *round_key)
	{
	uint8_t index;
	
	for (index = 0U;
		 index < 16U;
		 index++)
		 {
		 state[index] ^= round_key[index];	
	}
}

static void sub_bytes(uint8_t state[16]){
	uint8_t index;
	
	for (index = 0U;
		 index < 16U;
		 index++)
		 {
		 state[index] ^= aes_sbox[state[index]];	
	}
}

static void shift_rows(uint8_t state[16]){
    uint8_t temp;
	
    temp = state[1U];
    state[1U] = state[5U];
    state[5U] = state[9U];
    state[9U] = state[13U];
    state[13U] = temp;

    temp = state[2U];
    state[2U] = state[10U];
    state[10U] = temp;

    temp = state[6U];
    state[6U] = state[14U];
    state[14U] = temp;

    temp = state[3U];
    state[3U] = state[15U];
    state[15U] = state[11U];
    state[11U] = state[7U];
    state[7U] = temp;
}

static void mix_columns(
    uint8_t state[16])
{
    uint8_t column;
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;


    for (column = 0U;
         column < 4U;
         column++)
    {
        uint8_t index;

        index = column * 4U;

        a = state[index];
        b = state[index + 1U];
        c = state[index + 2U];
        d = state[index + 3U];


        state[index] =
            xtime(a) ^
            (xtime(b) ^ b) ^
            c ^
            d;

        state[index + 1U] =
            a ^
            xtime(b) ^
            (xtime(c) ^ c) ^
            d;
			
		state[index + 2U] =
            a ^
            b ^
            xtime(c) ^
            (xtime(d) ^ d);

        state[index + 3U] =
            (xtime(a) ^ a) ^
            b ^
            c ^
            xtime(d);
    }
}

uint8_t aescrypt_aes_sbox_lookup(
    uint8_t value)
{
    return aes_sbox[value];
}


/**********************************************************
 * AES-128 Block Encryption
 **********************************************************/

void aescrypt_software_encrypt_block(
    const uint8_t input[16],
    uint8_t output[16],
    const uint8_t round_keys[176])
{

    uint8_t state[16];
    uint8_t round;


    for (round = 0U;
         round < 16U;
         round++)
    {
        state[round] = input[round];
    }


    add_round_key(
        state,
        round_keys);


    for (round = 1U;
         round < 10U;
         round++)
    {
        sub_bytes(state);

        shift_rows(state);

        mix_columns(state);

        add_round_key(
            state,
            &round_keys[round * 16U]);
    }

    sub_bytes(state);

    shift_rows(state);

    add_round_key(
        state,
        &round_keys[160U]);


    for (round = 0U;
         round < 16U;
         round++)
    {
        output[round] = state[round];
    }
}


/**********************************************************
 * End of File
 **********************************************************/