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
    T_OPEN_ANGLE, // <
    T_CLOSE_ANGLE, // >
    T_OPEN_SINGLE, // '
    T_CLOSE_SINGLE, // '
    T_OPEN_DOUBLE, // "
    T_CLOSE_DOUBLE, // "

    T_GRAVE, // `
    T_PERCENT, // %
    T_SEMICOLON, // ;
    T_QMARK, // ?
    T_COMMA, // ,
    T_FIELD, // .
    T_ESCAPE, // \

    T_REF, // @
    T_DEREF, // $

    T_ADD, // +
    T_SUB, // -
    T_MUL, // *
    T_DIV, // /

    T_CONST_TYPE, // :
    T_MUT_TYPE, // ::
    T_ASSIGN, // =
    T_RETURN_TYPE, // ->

    T_EQ, // ==
    T_NEQ, // !=
    T_LT, // <
    T_LEQ, // <=
    T_GT, // >
    T_GTE, // >=
    T_NOT, // not
    T_AND, // and
    T_OR, // or

    T_BIT_NOT, // !
    T_BIT_XOR, // ^
    T_BIT_AND, // &
    T_BIT_OR, // |
    T_SHIFT_LEFT, // <<
    T_SHIFT_RIGHT, // >>

    // types
    T_I8, // i8
    T_U8, // u8
    T_I16, // i16
    T_U16, // u16
    T_I32, // i32
    T_U32, // u32
    T_I64, // i64
    T_U64, // u64

    // Keywords
    T_LET, // let
    T_IF, // if
    T_ELSE, // else
    T_ELIF, // elif
    T_FOR, // for
    T_WHILE, // while
    T_GOTO, // goto
    T_STRUCT, // struct
    T_FUNC, // func
    T_RETURN, // return
    T_BREAK, // break
    T_CONTINUE, // continue
    T_SWITCH, // match
    T_CASE, // case
    T_DEFINE, // define
    T_PRINT, // print

    T_COUNT
} Id;

typedef uint32_t TokenId;

#endif
