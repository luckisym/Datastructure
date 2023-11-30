/*
 * Author: Saleeman Mahamud
 * Student number: 14932458
 * Study: Computer Science
 *
 * This file contains implementations of various hash functions for testing performance.
 * The included hash functions are:
 *   - hash_too_simple: A simple hash function
 *   - murmur3_32: Implementation of the MurmurHash3 algorithm for 32-bit hash values.
 *   - adler32: Implementation of the Adler-32 checksum algorithm.
 *   - Fletcher16: Implementation of the Fletcher-16 checksum algorithm.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "hash_func.h"

const uint32_t MOD_ADLER = 65521;

unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long)*str;
}

static inline uint32_t murmur_32_scramble(uint32_t k) {
    k ^= (k >> 16);
    k *= 0x85ebca6b;
    k ^= (k >> 13);
    k *= 0xc2b2ae35;
    k ^= (k >> 16);
    return k;
}

uint32_t murmur3_32(const unsigned char* key, size_t len) {
    // Using a fixed seed value (e.g., 0)
    uint32_t h = 0;
    uint32_t k;

    /* Read in groups of 4. */
    for (size_t i = len >> 2; i; i--) {
        memcpy(&k, key, sizeof(uint32_t));
        key += sizeof(uint32_t);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    /* Read the rest. */
    k = 0;
    for (size_t i = len & 3; i; i--) {
        k <<= 8;
        k |= key[i - 1];
    }

    h ^= murmur_32_scramble(k);

    /* Finalize. */
    h ^= (uint32_t) len;
    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);

    return h;
}

uint32_t adler32(const unsigned char *data, size_t len) {
    uint32_t sum1 = 1;
    uint32_t sum2 = 0;
    size_t index;

    // Process each byte of the data in order
    for (index = 0; index < len; ++index) {
        sum1 = (sum1 + data[index]) % MOD_ADLER;
        sum2 = (sum2 + sum1) % MOD_ADLER;
    }

    return (uint32_t)((sum2 << 16) | sum1);
}

uint16_t Fletcher16( const unsigned char *data, size_t count ) {
   uint16_t sum1 = 0;
   uint16_t sum2 = 0;
   size_t index;

   for (index = 0; index < count; ++index) {
       sum1 = (uint16_t)((sum1 + data[index]) % 255);
       sum2 = (uint16_t)((sum2 + sum1) % 255);
   }

   return (sum2 << 8) | sum1;
}

unsigned long hash_fletcher(const unsigned char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }

    return (unsigned long)Fletcher16(str, len);
}

unsigned long hash_adler(const unsigned char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }

    return (unsigned long)adler32(str, len);
}

unsigned long hash_murmur(const unsigned char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }

    return (unsigned long)murmur3_32(str, len);
}
