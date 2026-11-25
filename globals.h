#ifndef GLOBALS_H
#define GLOBALS_H

#define U64(c) UINT64_C(c)
#define U32(c) UINT32_C(c)

#define report_error(c) fprintf(stderr, "[Error Report] %s %s:%d\n", #c, __FILE__, __LINE__)

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

typedef uint32_t TokenId;

#endif
