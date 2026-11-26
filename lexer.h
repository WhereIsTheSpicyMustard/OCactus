#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <stddef.h>
#include "globals.h"

void lexer_clean(uchar* restrict buf, uint32_t* restrict size);
int lexer_tokenize(uchar* restrict buf, const size_t buf_size);

#endif
