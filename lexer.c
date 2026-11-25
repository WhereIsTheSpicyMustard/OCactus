#include "lexer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "globals.h"

#define SP 32

#define MODE_NORMAL 0
#define MODE_COMMENT 1

static const uint8_t WHITESPACE[256] = {[' '] = 1, ['\t'] = 1, ['\n'] = 1};
static const uint8_t PUNCTUATION[256] = {
    ['!'] = 1, ['"']  = 1, ['#'] = 1, ['$']  = 1, ['%'] = 1,
    ['&'] = 1, ['\''] = 1, ['('] = 1, [')']  = 1, ['*'] = 1,
    ['+'] = 1, [',']  = 1, ['-'] = 1, ['.']  = 1, ['/'] = 1,
    [':'] = 1, [';']  = 1, ['<'] = 1, ['=']  = 1, ['>'] = 1,
    ['?'] = 1, ['@']  = 1, ['['] = 1, ['\\'] = 1, [']'] = 1,
    ['^'] = 1, ['{']  = 1, ['|'] = 1, ['}']  = 1, ['~'] = 1
};

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

#define TOKEN_MAX_SIZE 64

TokenId* lexer_tokenize(uchar* restrict buf, const size_t buf_size)
{
    assert(NULL != buf);

    size_t cap = 64;
    size_t size = 0;
    TokenId* tokens = malloc(cap * sizeof(TokenId));
    if (NULL == tokens) return NULL;

    uchar token[TOKEN_MAX_SIZE];
    size_t token_size = 0;

    for (size_t i = 0; i < buf_size; ++i, ++size) {
        if (size >= cap) {
            cap *= 2;
            if (size >= cap) { // overflow check
                free(tokens);
                return NULL;
            }

            TokenId* temp = realloc(tokens, cap * sizeof(TokenId));
            if (NULL == temp) {
                free(tokens);
                return NULL;
            }
            tokens = temp;
        }

        switch (buf[i]) {
            case '(': tokens[size] = T_OPEN_ROUND; continue;
            case ')': tokens[size] = T_CLOSE_ROUND; continue;
            case '[': tokens[size] = T_OPEN_SQUARE; continue;
            case ']': tokens[size] = T_CLOSE_SQUARE; continue;
            case '{': tokens[size] = T_OPEN_CURLY; continue;
            case '}': tokens[size] = T_CLOSE_CURLY; continue;
            case ';': tokens[size] = T_END; continue;
            case '?': tokens[size] = T_COND_JUMP; continue;
            case '.': tokens[size] = T_LABEL; continue;
            case '+': tokens[size] = T_ADD; continue;
            case '-': tokens[size] = T_SUB; continue;
            case '*': tokens[size] = T_MUL; continue;
            case '/': tokens[size] = T_DIV; continue;
            case '%': tokens[size] = T_MOD; continue;
            case '=': tokens[size] = T_EQ; continue;
            case '!': tokens[size] = T_NOT; continue;
            case '&': tokens[size] = T_AND; continue;
            case '|': tokens[size] = T_OR; continue;
            case '~': tokens[size] = T_BIT_NOT; continue;
            case '^': tokens[size] = T_BIT_XOR; continue;
            case '$': tokens[size] = T_BIT_AND; continue;
            case '@': tokens[size] = T_BIT_OR; continue;
            case ':': tokens[size] = T_ASSIGN; continue;

                      // special
            case '<':
                    tokens[size] = T_LT;
                    if (i < buf_size - 1) // if its not the last one
                        if (buf[i + 1] == '<') {
                            tokens[size] = T_SHIFT_LEFT;
                            ++i; // skip next
                        }
                    continue;
            case '>': 
                    tokens[size] = T_GT;
                    if (i < buf_size - 1) // if its not the last one
                        if (buf[i + 1] == '>') {
                            ++i; // skip next
                            tokens[size] = T_SHIFT_RIGHT;
                        }
                    continue;

            default: 
                    if (!is_alpha_numeric(buf[i])) {
                        free(tokens);
                        return NULL;
                    }

                    if (i > 0)
                       if (is_alpha_numeric(buf[i - 1]))
                           continue;

                    token_size = 0;
                    for (; i < buf_size; ++i) {
                        if (!is_alpha_numeric(buf[i])) {
                            break;
                        }

                        if (token_size >= TOKEN_MAX_SIZE) {
                            free(tokens);
                            return NULL;
                        }
                        token[token_size] = buf[i];
                        ++token_size;
                    }
                      continue;
        }
    }

    return tokens;
}
