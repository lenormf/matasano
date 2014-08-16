/*
 * fixed_xor.c for matasano
 * by lenormf
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "utils.h"

eMatasanoError fixed_xor(char **out, char const *text, char const *key, size_t sz) {
    size_t i, j;
    uint8_t *text_n8;
    uint8_t *key_n8;

    if (!*out || !text || !key || !sz) {
        return ERR_INVALID_ARGUMENT;
    } else if (sz % 1) {
        return ERR_ALIGN_ERROR;
    }

    text_n8 = (uint8_t*)strndup(text, sz);
    if (!text_n8) {
        return ERR_ALLOC_ERROR;
    }

    key_n8 = (uint8_t*)strndup(key, sz);
    if (!key_n8) {
        return ERR_ALLOC_ERROR;
    }

    if (str_hex_to_n8(text_n8)) {
        return ERR_NOT_HEXADECIMAL;
    }

    if (str_hex_to_n8(key_n8)) {
        return ERR_NOT_HEXADECIMAL;
    }

    *out = malloc(sz);
    if (!*out) {
        return ERR_ALLOC_ERROR;
    }

    for (i = 0, j = 0; i < sz / 2; i++, j += 2) {
        uint8_t const x = text_n8[i] ^ key_n8[i];
        char *c = *out + j;

        c[0] = idx_to_hex(x / 16);
        c[1] = idx_to_hex(x % 16);
    }

    free(text_n8);
    free(key_n8);

    return ERR_NO_ERROR;
}

int main(void) {
    char const ref[] = "746865206b696420646f6e277420706c6179";
    char const text[] = "1c0111001f010100061a024b53535009181c";
    char const key[] = "686974207468652062756c6c277320657965";
    char *out;
    eMatasanoError err;

    err = fixed_xor(&out, text, key, sizeof(text) - 1);
    if (ERR_NO_ERROR != err) {
        fprintf(stderr, "Error: %s\n", error_to_str(err));
        return (int)err;
    }

    printf("\"%s\" === \"%s\" (cmp: %d)\n", ref, out, strcmp(ref, out));

    free(out);

    return 0;
}
