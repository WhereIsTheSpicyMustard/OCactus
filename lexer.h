#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <stddef.h>
#include "globals.h"

extern uchar* token_strings;

void lexer_clean(uchar* restrict buf, uint32_t* restrict size);
uint32_t* lexer_tokenize(uchar* restrict buf, const size_t buf_size);

#endif
