#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <stddef.h>
#include "globals.h"

void lexer_clean(char* restrict buf, uint32_t* restrict size);
TokenId* lexer_tokenize(char* restrict buf, const size_t buf_size);

#endif
