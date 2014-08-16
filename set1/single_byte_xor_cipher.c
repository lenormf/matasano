/*
 * single_byte_xor_cipher.c for matasano
 * by lenormf
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "error.h"
#include "macro.h"
#include "utils.h"

typedef enum {
    LTR_E,
    LTR_T,
    LTR_A,
    LTR_O,
    LTR_I,
    LTR_SPC,
} eScoreLetter;

struct cipher_score {
    uint8_t key;
    unsigned int scores[6];
    unsigned int total_score;
};

eMatasanoError single_byte_xor_cipher(uint8_t *key, char const *cipher, size_t sz) {
    struct cipher_score best_score;
    size_t text_n8_sz;
    uint8_t *text_n8;
    uint8_t c;

    if (!key || !cipher || !sz) {
        return ERR_INVALID_ARGUMENT;
    } else if (sz % 1) {
        return ERR_ALIGN_ERROR;
    }

    text_n8 = (uint8_t*)strndup(cipher, sz);
    if (!text_n8) {
        return ERR_ALLOC_ERROR;
    }

    text_n8_sz = sz / 2;
    if (str_hex_to_n8(text_n8)) {
        return ERR_NOT_HEXADECIMAL;
    }

    BZERO(&best_score, sizeof(struct cipher_score));
    for (c = 0; c < 0xFF; c++) {
        struct cipher_score score;
        unsigned int i;
        uint8_t *buffer;

        buffer = malloc(text_n8_sz);
        if (!buffer) {
            return ERR_ALLOC_ERROR;
        }
        memcpy(buffer, text_n8, text_n8_sz);

        BZERO(&score, sizeof(struct cipher_score));
        score.key = c;
        for (i = 0; i < (unsigned int)text_n8_sz; i++) {
            buffer[i] ^= c;

            if (!PRINTABLE(buffer[i])) {
                break;
            } else {
                switch (LOWERCASE(buffer[i])) {
                    case 'e': score.scores[LTR_E]++; break;
                    case 't': score.scores[LTR_T]++; break;
                    case 'a': score.scores[LTR_A]++; break;
                    case 'o': score.scores[LTR_O]++; break;
                    case 'i': score.scores[LTR_I]++; break;
                    case 0x20: score.scores[LTR_SPC]++; break;
                    default: break;
                }
            }
        }
        if (i >= (unsigned int)text_n8_sz) {
            uint8_t ign;

            ign = 0;
            for (i = 0; i < ARRAY_SIZE(score.scores) - 1; i++) {
                if (score.scores[i] >= text_n8_sz / 2) {
                    ign = 1;
                    break;
                }

                score.total_score += score.scores[i];
            }
            if (!ign && score.scores[LTR_SPC]) {
                if (score.total_score > best_score.total_score) {
                    memcpy(&best_score, &score, sizeof(struct cipher_score));
                }
            }
        }

        free(buffer);
    }

    *key = best_score.key;

    free(text_n8);

    return ERR_NO_ERROR;
}

// ref: key=0x58, text="Cooking MC's like a pound of bacon"
int main(void) {
    char const cipher[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    uint8_t text[sizeof(cipher)];
    eMatasanoError err;
    unsigned int i;
    uint8_t k;

    memcpy(text, cipher, sizeof(cipher));
    if (str_hex_to_n8(text)) {
        return 1;
    }
    text[sizeof(text) / 2] = 0;

    err = single_byte_xor_cipher(&k, cipher, sizeof(cipher) - 1);
    if (ERR_NO_ERROR != err) {
        fprintf(stderr, "Error: %s\n", error_to_str(err));
        return 1;
    }

    for (i = 0; i < sizeof(cipher) / 2; i++) {
        text[i] ^= k;
    }

    printf("key: %hhX, text: %s\n", k, (char*)text);

    return 0;
}
