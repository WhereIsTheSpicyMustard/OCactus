#include "lexer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "globals.h"

#define SP 32

#define MODE_NORMAL 0
#define MODE_COMMENT 1

static const uint8_t WHITESPACE[128] = {[' '] = 1, ['\t'] = 1, ['\n'] = 1};
static const uint8_t PUNCTUATION[128] = {
    ['!'] = 1, ['"']  = 1, ['#'] = 1, ['$']  = 1, ['%'] = 1,
    ['&'] = 1, ['\''] = 1, ['('] = 1, [')']  = 1, ['*'] = 1,
    ['+'] = 1, [',']  = 1, ['-'] = 1, ['.']  = 1, ['/'] = 1,
    [':'] = 1, [';']  = 1, ['<'] = 1, ['=']  = 1, ['>'] = 1,
    ['?'] = 1, ['@']  = 1, ['['] = 1, ['\\'] = 1, [']'] = 1,
    ['^'] = 1, ['{']  = 1, ['|'] = 1, ['}']  = 1, ['~'] = 1
};

static inline int is_punctuation(const char c)
{
    return ((size_t)c < sizeof(PUNCTUATION)) && (PUNCTUATION[c]);
}

static inline int is_white_space(const char c)
{
    return ((size_t)c < sizeof(WHITESPACE)) && (WHITESPACE[c]);
}

void lexer_clean(char* restrict buf, uint32_t* restrict size)
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

TokenId* lexer_tokenize(char* restrict buf, const size_t buf_size)
{
    assert(NULL != buf);

    size_t cap = 64;
    size_t size = 0;
    TokenId* tokens = malloc(cap * sizeof(TokenId));
    if (NULL == tokens) return NULL;

    for (size_t i = 0; i < buf_size; ++i, ++size) {
        if (size >= cap) {
            cap *= 2;
            TokenId* temp = realloc(tokens, cap * sizeof(TokenId));
            if (NULL == temp) {
                free(tokens);
                return NULL;
            }
            tokens = temp;
        }

        switch (buf[i]) {
            case '(':
                tokens[size] = T_OPEN_ROUND;
                continue;
            case ')':
                tokens[size] = T_CLOSE_ROUND;
                continue;
            case '[':
                tokens[size] = T_OPEN_SQUARE;
                continue;
            case ']':
                tokens[size] = T_CLOSE_SQUARE;
                continue;
            case '{':
                tokens[size] = T_OPEN_CURLY;
                continue;
            case '}':
                tokens[size] = T_CLOSE_CURLY;
                continue;
            case '<':
                continue;
            case '>':
                continue;
            case '\'':
                continue;
            case '\"':
                continue;
            case '`':
                continue;
            case '%':
                continue;
            case ';':
                continue;
            case '?':
                continue;
            case ',':
                continue;
            case '.':
                continue;
            case '\\':
                continue;
            case '@':
                continue;
            case '$':
                continue;
            case '+':
                continue;
            case '-':
                continue;
            case '*':
                continue;
            case '/':
                continue;
            case ':':
                continue;
            case '=':
                continue;
            case '!':
                continue;
            case '^':
                continue;
            case '&':
                continue;
            case '|':
                continue;
        }
    }

    return tokens;
}
