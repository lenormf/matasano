/*
 * hex_to_base64.c for matasano
 * by lenormf
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "macro.h"
#include "utils.h"

#define UNSAFE_C_TO_B64(c) ((uint8_t)idx_to_b64(c))

eMatasanoError str_hex_to_base64(char **b64, char const *s, size_t sz) {
    uint8_t *n8;
    char *out;
    size_t b64_sz;
    size_t i, j;

    if (!s || !sz) {
        return ERR_INVALID_ARGUMENT;
    } else if (sz % 1) {
        return ERR_ALIGN_ERROR;
    }

    b64_sz = sz / 2 * 4 / 3 + 1;
    *b64 = malloc(b64_sz);
    if (!*b64) {
        return ERR_ALLOC_ERROR;
    }
    out = *b64;

    n8 = (uint8_t*)strdup(s);
    if (!n8) {
        return ERR_ALLOC_ERROR;
    }

    if (str_hex_to_n8(n8)) {
        return ERR_NOT_HEXADECIMAL;
    }
    n8[sz / 2] = 0;

    // padding not supported
    for (i = 0, j = 0; i < sz / 2; i += 3, j += 4) {
        out[j] = UNSAFE_C_TO_B64((n8[i] & 0xFC) >> 2);
        out[j + 1] = UNSAFE_C_TO_B64((n8[i] & 0x03) << 4 | (n8[i + 1] & 0xF0) >> 4);
        out[j + 2] = UNSAFE_C_TO_B64((n8[i + 1] & 0x0F) << 2 | (n8[i + 2] & 0xC0) >> 6);
        out[j + 3] = UNSAFE_C_TO_B64(n8[i + 2] & 0x3F);
    }
    out[j] = 0;

    free(n8);

    return ERR_NO_ERROR;
}

#ifndef STRIP_TEST_MAIN
int main(void) {
    char const ref[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    char const in[] =  "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char *out;
    eMatasanoError err;

    err = str_hex_to_base64(&out, in, sizeof(in) - 1);
    if (ERR_NO_ERROR != err) {
        fprintf(stderr, "Error: %s\n", error_to_str(err));
        return (int)err;
    }

    printf("\"%s\" === \"%s\" (cmp: %d)\n", ref, out, strcmp(ref, out));

    free(out);

    return 0;
}
#endif
