/*
 * error.h for matasano
 * by matasano
 */

#pragma once

typedef enum {
    ERR_NO_ERROR,
    ERR_INVALID_ARGUMENT,
    ERR_ALLOC_ERROR,

    ERR_ALIGN_ERROR,
    ERR_NOT_HEXADECIMAL,
} eMatasanoError;

char const *error_to_str(eMatasanoError);
