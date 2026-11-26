#include "lexer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "globals.h"

#define SP 32

#define MODE_NORMAL 0
#define MODE_COMMENT 1

#define TOKEN_MAX_SIZE 64

static const uint8_t WHITESPACE[256] = {[' '] = 1, ['\t'] = 1, ['\n'] = 1};
static const uint8_t PUNCTUATION[256] = {
    ['!'] = 1, ['"']  = 1, ['#'] = 1, ['$']  = 1, ['%'] = 1,
    ['&'] = 1, ['\''] = 1, ['('] = 1, [')']  = 1, ['*'] = 1,
    ['+'] = 1, [',']  = 1, ['-'] = 1, ['.']  = 1, ['/'] = 1,
    [':'] = 1, [';']  = 1, ['<'] = 1, ['=']  = 1, ['>'] = 1,
    ['?'] = 1, ['@']  = 1, ['['] = 1, ['\\'] = 1, [']'] = 1,
    ['^'] = 1, ['{']  = 1, ['|'] = 1, ['}']  = 1, ['~'] = 1
};

CharArray token_strings;
Int32Array tokens;
Int32Array token_map;

static inline int is_alpha_numeric(const uchar c)
{
    return (
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') ||
            c == '_'
            );
}

static inline int is_punctuation(const uchar c)
{
    return PUNCTUATION[c];
}

static inline int is_white_space(const uchar c)
{
    return WHITESPACE[c];
}

void lexer_clean(uchar* restrict buf, uint32_t* restrict size)
{
    assert(NULL != buf);
    assert(NULL != size);

    // remove comments
    for (uint32_t i = 0, mode = MODE_NORMAL; i < *size; ++i) {
        if (mode == MODE_NORMAL) {
            if (buf[i] == '\\') {
                ++i; // skip next char
                continue;
            }
            if (buf[i] != '#')
                continue;
            mode = MODE_COMMENT;
            buf[i] = SP;
        } else {
            if (buf[i] == '\n')
                mode = MODE_NORMAL;
            else
                buf[i] = SP;
        }
    }

    // trim leading whitespace
    uint32_t write = 0;
    for (uint32_t read = 0; read < *size; ++read) {
        if (write == 0 && is_white_space(buf[read]))
            continue;
        buf[write++] = buf[read];
    }

    *size = write;

    // collapse whitespace
    write = 1;
    for (uint32_t read = 1; read < *size; ++read) {
        if (is_white_space(buf[read])) {
            if (!is_white_space(buf[read - 1]))
                buf[write++] = SP;
            continue;
        }
        buf[write++] = buf[read];
    }

    *size = write;

    // remove whitespace
    write = 0;
    for (uint32_t read = 2; read < *size; ++read) {
        buf[write++] = buf[read - 2];
        if (is_punctuation(buf[read - 2]) || is_punctuation(buf[read])) {
            if (is_white_space(buf[read - 1]))
                ++read;
        }
    }

    *size = write;
}


int lexer_tokenize(uchar* restrict buf, const size_t buf_size)
{
    assert(NULL != buf);
    assert(buf_size > 1);

    array_init(tokens);
    array_init(token_strings);
    array_init(token_map);

    uchar token[TOKEN_MAX_SIZE];
    size_t token_size = 0;


    for (size_t i = 0; i < buf_size; ++i) {
        switch (buf[i]) {
            case '(': array_append(tokens, T_OPEN_ROUND); continue;
            case ')': array_append(tokens, T_CLOSE_ROUND); continue;
            case '[': array_append(tokens, T_OPEN_SQUARE); continue;
            case ']': array_append(tokens, T_CLOSE_SQUARE); continue;
            case '{': array_append(tokens, T_OPEN_CURLY); continue;
            case '}': array_append(tokens, T_CLOSE_CURLY); continue;
            case ';': array_append(tokens, T_END); continue;
            case '?': array_append(tokens, T_COND_JUMP); continue;
            case '.': array_append(tokens, T_LABEL); continue;
            case '+': array_append(tokens, T_ADD); continue;
            case '-': array_append(tokens, T_SUB); continue;
            case '*': array_append(tokens, T_MUL); continue;
            case '/': array_append(tokens, T_DIV); continue;
            case '%': array_append(tokens, T_MOD); continue;
            case '=': array_append(tokens, T_EQ); continue;
            case '!': array_append(tokens, T_NOT); continue;
            case '&': array_append(tokens, T_AND); continue;
            case '|': array_append(tokens, T_OR); continue;
            case '~': array_append(tokens, T_BIT_NOT); continue;
            case '^': array_append(tokens, T_BIT_XOR); continue;
            case '$': array_append(tokens, T_BIT_AND); continue;
            case '@': array_append(tokens, T_BIT_OR); continue;
            case ':': array_append(tokens, T_ASSIGN); continue;

                      // special
            case '<':
                    if (i < buf_size - 1) // if its not the last one
                        if (buf[i + 1] == '<') {
                            ++i; // skip next
                            array_append(tokens, T_SHIFT_LEFT);
                            continue;
                        }
                    array_append(tokens, T_LT);
                    continue;
            case '>': 
                    if (i < buf_size - 1) // if its not the last one
                        if (buf[i + 1] == '>') {
                            ++i; // skip next
                            array_append(tokens, T_SHIFT_RIGHT);
                            continue;
                        }
                    array_append(tokens, T_GT);
                    continue;

            default: 
                    if (!is_alpha_numeric(buf[i]))
                        return 1;

                    if (i > 0)
                       if (is_alpha_numeric(buf[i - 1]))
                           continue;

                    token_size = 0;
                    for (; i < buf_size; ++i) {
                        if (!is_alpha_numeric(buf[i])) {
                            --i;
                            break;
                        }

                        if (token_size >= TOKEN_MAX_SIZE) 
                            return 1;
                        token[token_size] = buf[i];
                        ++token_size;
                    }


                    continue;
        }
    }

    return 0;
}
