#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

#define U64(c) UINT64_C(c)
#define U32(c) UINT32_C(c)

#define report_error(c) fprintf(stderr, "[Error Report] %s %s:%d\n", #c, __FILE__, __LINE__)

typedef unsigned char uchar;

typedef enum {
    T_OPEN_ROUND, // (
    T_CLOSE_ROUND, // )
    T_OPEN_SQUARE, // [
    T_CLOSE_SQUARE, // ]
    T_OPEN_CURLY, // {
    T_CLOSE_CURLY, // }

    T_END, // ;
    T_COND_JUMP, // ?
    T_LABEL, // .

    T_ADD, // +
    T_SUB, // -
    T_MUL, // *
    T_DIV, // /
    T_MOD, // %

    T_EQ, // =
    T_LT, // <
    T_GT, // >
    T_NOT, // !
    T_AND, // &
    T_OR, // |

    T_BIT_NOT, // ~
    T_BIT_XOR, // ^
    T_BIT_AND, // $
    T_BIT_OR, // @
    T_SHIFT_LEFT, // <<
    T_SHIFT_RIGHT, // >>

    T_ASSIGN, // :
    T_INDEX, // 

    // types
    T_I8, // i8
    T_U8, // u8
    T_I16, // i16
    T_U16, // u16
    T_I32, // i32
    T_U32, // u32
    T_I64, // i64
    T_U64, // u64

    T_COUNT
} Id;


typedef struct {
    uchar* data;
    size_t size;
    size_t capacity;
} CharArray;

typedef struct {
    uint32_t* data;
    size_t size;
    size_t capacity;
} Int32Array;

#define array_append(arr, x) do { \
    __typeof__(*(arr).data) aa_item = (x); \
    if ((arr).size >= (arr).capacity) { \
        if ((arr).capacity == 0) { \
            assert((arr).data == NULL); \
            assert((arr).size == 0); \
            (arr).capacity = 32; \
        } \
        (arr).capacity *= 2; \
        if ((arr).size >= (arr).capacity) { \
            fprintf(stderr, "%s:%d: array_append: capacity overflow\n", __FILE__, __LINE__); \
            abort(); \
        } \
        __typeof__((arr).data) aa_temp = realloc((arr).data, (arr).capacity * sizeof(*((arr).data))); \
        if (aa_temp == NULL) { \
            fprintf(stderr, "%s:%d: array_append: realloc failed\n", __FILE__, __LINE__); \
            abort(); \
        } \
        (arr).data = aa_temp; \
    } \
    (arr).data[(arr).size] = aa_item; \
    ++((arr).size); \
} while (0)


#endif
