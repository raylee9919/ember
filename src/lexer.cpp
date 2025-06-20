#include "basic.h"
#include "os.h"
#include "lexer.h"
#include "parser.h"

bool starts_identifier(int c) {
    if (isalpha(c)) return true;
    if (c == '_')   return true;
    return false;
}

bool continues_identifier(int c) {
    if (isalnum(c) || (c == '_')) return true;
    return false;
}

bool starts_number(int c) {
    if (isdigit(c)) return true;
    return false;
}

bool string_equals(char *string_to_compare, int string_length, const char *string_literal) {
    for (int i = 0; i < string_length; ++i) {
        if (string_to_compare[i] != string_literal[i]) {
            return false; 
        }
    }
    return true;
}

bool iswhitespace(int c) {
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t') return true;
    return false;
}

int Lexer::peek_next_character() {
    assert(input.data != NULL);

    if (input_cursor >= input.count) {
        return -1;
    }

    int result = input.data[input_cursor];
    return result;
}

void Lexer::eat_character() {
    if (input.data[input_cursor] == '\n') {
        ++current_line_number;
        current_character_index = 0; // Will get incremented to 1 below.
    }

    ++input_cursor;
    ++current_character_index;
}

void Lexer::unwind_one_character() {
    assert(input_cursor != 0);

    --input_cursor;
    --current_character_index;
}

void Lexer::init(Parser *parser) {
    parser->lexer = this;
}

void Lexer::set_input_from_file(void *file_ptr) {
    assert(file_ptr);

    File_Handle *f = (File_Handle *)file_ptr;

    void *data;
    size_t length = os_read_entire_file(f, &data);
    if (length < 0) return;

    if (should_free_input) free(input.data);
    should_free_input = true;

    Newstring s;
    s.data = (u8 *)data;
    s.count = length;

    input = s;
    input_cursor = 0;

    current_line_number = 1;
    current_character_index = 1;
}

void Lexer::push_single_token(Token_Type type) {
    while (((next_index_to_write + 1) & (MAX_CONCURRENT_TOKENS - 1)) == next_index_to_read) {};
    Token tk = Token{};
    tk.type = type;
    tokens[next_index_to_write] = tk;
    next_index_to_write = (next_index_to_write + 1) & (MAX_CONCURRENT_TOKENS - 1);
}

void Lexer::push_token(Token token) {
    while (((next_index_to_write + 1) & (MAX_CONCURRENT_TOKENS - 1)) == next_index_to_read) {};
    tokens[next_index_to_write] = token;
    next_index_to_write = (next_index_to_write + 1) & (MAX_CONCURRENT_TOKENS - 1);
}

void Lexer::tokenize() {
    int c = peek_next_character();

    while (input_cursor < input.count) {
        switch (c) {
            case '+': {
                eat_character();
                c = peek_next_character();
                if (c == '=') {
                    assert(0);
                } else {
                    push_single_token((Token_Type)'+');
                }
            } break;

            case '-': {
                eat_character();
                c = peek_next_character();
                if (c == '=') {
                    assert(0);
                } else {
                    push_single_token((Token_Type)'-');
                }
            } break;

            case '*': {
                eat_character();
                c = peek_next_character();
                if (c == '=') {
                    assert(0);
                } else {
                    push_single_token((Token_Type)'*');
                }
            } break;

            case '/': {
                eat_character();
                c = peek_next_character();
                if (c == '=') {
                    assert(0);
                } else {
                    push_single_token((Token_Type)'/');
                }
            } break;

            case ';': {
                eat_character();
                push_single_token((Token_Type)';');
            }break;

            default: {
                if (isalpha(c)) {
                    int keyword_length = get_keyword_length();
                    switch (keyword_length) {
                        case 2: {
                            if (string_equals((char *)input.data + input_cursor, keyword_length, "if")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "else")) {
                            }
                        } break;

                        case 3: {
                            if (string_equals((char *)input.data + input_cursor, keyword_length, "for")) {
                            }
                        } break;

                        case 4: {
                            if (string_equals((char *)input.data + input_cursor, keyword_length, "true")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "enum")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "case")) {
                            }
                        } break;

                        case 5: {
                            if (string_equals((char *)input.data + input_cursor, keyword_length, "false")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "while")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "union")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "break")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "using")) {
                            }
                        } break;

                        case 6: {
                            if (string_equals((char *)input.data + input_cursor, keyword_length, "return")) {
                            } else if (string_equals((char *)input.data + input_cursor, keyword_length, "struct")) {
                            }
                        } break;

                        case 8: {
                            if (string_equals((char *)input.data + input_cursor, keyword_length, "continue")) {
                            }
                        } break;

                        default: {
                            assert(0);
                        } break;
                    }
                } else if (isdigit(c)) {
                    push_token(parse_number());
                } else if (iswhitespace(c)) {
                    eat_character();
                } else {
                    assert(0);
                }
            } break;
        }
        c = peek_next_character();
    }

    push_single_token(TOKEN_END_OF_INPUT);
}

int Lexer::get_keyword_length() {
    int result = 0;

    int c = peek_next_character();

    while (isalpha(c)) {
        eat_character();
        c = peek_next_character();
    }

    return result;
}

Token Lexer::parse_number() {
    Token num{};
    num.type = TOKEN_NUMBER;

    u64 d = 0;
    f64 f = 0.0;
    f64 i = 0.1;

    int c = peek_next_character();
    while (isdigit(c)) {
        d = d*10 + (c-'0');
        eat_character();
        c = peek_next_character();
    }

    if (c == '.') {
        eat_character();
        c = peek_next_character();
        while (isdigit(c)) {
            f += (i*(c-'0'));
            i *= 0.1;
            eat_character();
            c = peek_next_character();
        }

        num.float64_value = (f64)d + f;
    } else {
        num.integer_value = d;
    }

    return num;
}
