/*
 * macro.h for matasano
 * by lenormf
 */

#pragma once

#define LOWERCASE(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 0x20 : (c))
#define PRINTABLE(c) ((c) >= 0x20 && (c) <= 0x7E)
#define BZERO(x, s) memset(x, 0, s)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))
