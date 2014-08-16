/*
 * utils.c for matasano
 * by lenormf
 */

#include "utils.h"
#include "macro.h"

static char const g_base_16[] = "0123456789abcdef";
static char const g_base_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int find(char const *s, char c) {
    int i;

    for (i = 0; s[i]; i++) {
        if (s[i] == c) {
            return i;
        }
    }

    return -1;
}

static int str2_hex_to_n8(char const *s) {
    int n[2];

    n[0] = find(g_base_16, LOWERCASE(s[0]));
    if (n[0] < 0) {
        return -1;
    }

    n[1] = find(g_base_16, LOWERCASE(s[1]));
    if (n[1] < 0) {
        return -1;
    }

    return n[0] * 16 + n[1];
}

int str_hex_to_n8(uint8_t *s) {
    unsigned int i, j;

    for (i = 0, j = 0; s[j]; i++, j += 2) {
        int const n = str2_hex_to_n8((char*)s + j);

        if (n < 0) {
            return -1;
        }

        s[i] = (uint8_t)n;
    }

    return 0;
}

int idx_to_b64(uint8_t c) {
    return c >= sizeof(g_base_64) - 1 ? -1 : g_base_64[c];
}

int idx_to_hex(uint8_t c) {
    return c >= sizeof(g_base_16) - 1 ? -1 : g_base_16[c];
}
