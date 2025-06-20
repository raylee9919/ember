#pragma once

#include "basic.h"

const int MAX_TOKEN_LENGTH = 512;
const int MAX_CONCURRENT_TOKENS = 8;

enum Token_Type : int16_t {
    // Mentally insert ASCII types here.

    TOKEN_BITWISE_XOR = '^',
    TOKEN_BITWISE_NOT = '~',
    TOKEN_BITWISE_AND = '&',
    TOKEN_BITWISE_OR  = '|',

    TOKEN_IDENT = 256,
    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_PLUSEQUALS,
    TOKEN_MINUSEQUALS,
    TOKEN_TIMESEQUALS,
    TOKEN_DIVEQUALS,
    TOKEN_MODEQUALS,
    TOKEN_ISEQUAL,
    TOKEN_ISNOTEQUAL,
    TOKEN_LOGICAL_AND,
    TOKEN_LOGICAL_OR,
    TOKEN_LESSEQUALS,
    TOKEN_GREATEREQUALS,

    TOKEN_RIGHT_ARROW,
    TOKEN_DOUBLE_DOT,
    TOKEN_DOUBLE_DOLLAR,

    TOKEN_SHIFT_LEFT,
    TOKEN_SHIFT_RIGHT,

    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_UNION,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_BREAK,
    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_USING,

    TOKEN_KEYWORD_ENUM,
    TOKEN_KEYWORD_TRUE,
    TOKEN_KEYWORD_FALSE,

    TOKEN_END_OF_INPUT,
    TOKEN_ERROR
};

struct Token {
    Token_Type type = TOKEN_ERROR;

    s32 l0 = 0;
    s32 c0 = 0;

    s32 l1 = -1;
    s32 c1 = -1;

    union {
        u64 integer_value;
        f32 float32_value;
        f64 float64_value;
        struct { u64 count; u8 *data; } string_value; // @Cleanup: This is kind of ugly.
    };

    u32 identifier_length;
    unsigned char token_scratch_buffer[MAX_TOKEN_LENGTH]; // @Robustness: Kind of lame, do something better!
};

struct Lexer {
    void init(struct Parser *parser);
    void set_input_from_file(void *file_ptr);
    int peek_next_character();
    void eat_character();
    void unwind_one_character();
    void tokenize();
    int get_keyword_length();
    void eat_token();
    void push_token(Token token);
    void push_single_token(Token_Type type);
    Token parse_number();


    s32 current_line_number;
    s32 current_character_index;

    // Ring buffer containing tokens produced by lexer.
    // Parser will consume in other thread.
    Token tokens[MAX_CONCURRENT_TOKENS];
    int next_index_to_write = 0; // Equivalent to head of ciruclar(ring) buffer.
    int next_index_to_read  = 0; //            to tail.

    Newstring input;
    u64 input_cursor;

    bool should_free_input;
};
