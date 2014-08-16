/*
 * error.c for matasano
 * by lenormf
 */

#include "error.h"

static char const *errors_ref[] = {
    "no error",
    "invalid argument",
    "allocation error",
    "alignment error",
    "not in hexadecimal notation",
};

char const *error_to_str(eMatasanoError err) {
    return errors_ref[(unsigned int)err];
}
