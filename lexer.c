#include "lexer.h"
#include <stddef.h>
#include <stdint.h>
#include "types.h"

#define SP 32

enum {
    MODE_NORMAL,
    MODE_ONELINE_COMMENT,
    MODE_MULTILINE_COMMENT
};

static const uint8_t WHITESPACE[128] = {[' '] = 1, ['\t'] = 1, ['\n'] = 1};
static const uint8_t PUNCTUATION[128] = {
    ['!'] = 1, ['"']  = 1, ['#'] = 1, ['$']  = 1, ['%'] = 1,
    ['&'] = 1, ['\''] = 1, ['('] = 1, [')']  = 1, ['*'] = 1,
    ['+'] = 1, [',']  = 1, ['-'] = 1, ['.']  = 1, ['/'] = 1,
    [':'] = 1, [';']  = 1, ['<'] = 1, ['=']  = 1, ['>'] = 1,
    ['?'] = 1, ['@']  = 1, ['['] = 1, ['\\'] = 1, [']'] = 1,
    ['^'] = 1, ['{']  = 1, ['|'] = 1, ['}']  = 1, ['~'] = 1
};

static inline int is_punctuation(const uchar c)
{
    return (c < sizeof(PUNCTUATION)) && (PUNCTUATION[c]);
}

static inline int is_white_space(const uchar c)
{
    return (c < sizeof(WHITESPACE)) && (WHITESPACE[c]);
}

void lexer_clean(uchar* restrict buf, uint32_t* restrict size)
{
    // remove comments
    int mode = MODE_NORMAL;
    for (uint32_t i = 0; (i + 2) < (*size); ++i) {
        switch (mode) {
            case MODE_NORMAL:
                if (buf[i] != '/' || buf[i + 1] != '/')
                    continue;
                mode = MODE_ONELINE_COMMENT;
                buf[i + 1] = SP;
                buf[i] = SP;
                if (buf[i + 2] != '/')
                    continue;
                mode = MODE_MULTILINE_COMMENT;
                buf[i + 2] = SP;
                continue;
            case MODE_ONELINE_COMMENT:
                if (buf[i] == '\n')
                    mode = MODE_NORMAL;
                else
                    buf[i] = SP;
                continue;
            case MODE_MULTILINE_COMMENT:
                if (buf[i] == '/' && buf[i + 1] == '/' && buf[i + 2] == '/') {
                    mode = MODE_NORMAL;
                    buf[i + 1] = SP;
                    buf[i + 2] = SP;
                }
                buf[i] = SP;
                continue;
        }
    }

    if (mode == MODE_MULTILINE_COMMENT) {
        buf[(*size) - 1] = SP;
        buf[(*size) - 2] = SP;
    } else if (mode == MODE_ONELINE_COMMENT && buf[(*size) - 2] != '\n') {
        buf[(*size) - 2] = SP;
        buf[(*size) - 1] = SP;
    }

    // collapse whitespace
    uint32_t write = 0;
    for (uint32_t read = 0; (read + 1) < (*size); ++read) {
        if (is_white_space(buf[read])) {
            if (is_white_space(buf[read + 1]))
                continue;
            buf[write++] = SP;
            continue;
        }
        buf[write++] = buf[read];
    }
    if (is_white_space(buf[*size - 1]))
        buf[*size - 1] = SP;
    buf[write++] = buf[*size - 1];

    // remove whitespace
    const uint32_t max = write;
    write = 0;
    for (uint32_t read = buf[0] == SP; (read + 2) < max; ++read) {
        if (buf[read + 1] == SP) {
            if (is_punctuation(buf[read]) || is_punctuation(buf[read + 2])) {
                buf[write++] = buf[read];
                ++read;
                continue;
            }
        }
        buf[write++] = buf[read];
    }

    if (buf[max - 2] != SP)
        buf[write++] = buf[max - 2];
    if (buf[max - 1] != SP)
        buf[write++] = buf[max - 1];

    *size = write;
}
