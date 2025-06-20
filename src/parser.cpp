#include "parser.h"
#include "lexer.h"
#include "ast.h"

void Parser::parse() {
    Token token = peek_next_token();

    switch (token.type) {
        case TOKEN_NUMBER: {
            Ast *expr = parse_expression(MIN_PREC);
        } break;

        case TOKEN_IDENT: {
            assert(0);
        } break;

        case TOKEN_END_OF_INPUT:
        return;

        default: {
            assert(0);
        } break;
    }
}

Token Parser::peek_next_token() {
    if (input_ended) {
        Token end_of_input{};
        end_of_input.type = TOKEN_END_OF_INPUT;
        return end_of_input;
    } else {
        while (lexer->next_index_to_read == lexer->next_index_to_write) {}
        Token token = lexer->tokens[lexer->next_index_to_read];
        if (token.type == TOKEN_END_OF_INPUT) input_ended = true;
        return token;
    }
}

void Parser::eat_token() {
    lexer->next_index_to_read = ((lexer->next_index_to_read + 1) & (MAX_CONCURRENT_TOKENS - 1));
}

Token Parser::get_next_token() {
    Token token = peek_next_token();
    eat_token();
    return token;
}

bool is_binary_operator(Token t) {
    if (t.type == '+' || t.type == '-' || t.type == '*' || t.type == '/' || t.type == '%') return true;
    return false;
}

int get_precedence(Token t) {
    if (t.type == '*' || t.type == '/') return 2;
    if (t.type == '+' || t.type == '-') return 1;
    else assert(0);
}

Ast *make_number(Token t) {
    Ast_Number *result = new Ast_Number();
    result->integer_value = t.integer_value;
    result->float32_value = t.float32_value;
    result->float64_value = t.float64_value;
    return result;
}

Ast *make_variable(Token t) {
    assert(0);
    return 0;
}

Ast_Operator *to_operator(Token t) {
    Ast_Operator *result = new Ast_Operator();
    if (t.type == '+') result->op = OPERATOR_PLUS;
    else if (t.type == '-') result->op = OPERATOR_MINUS;
    else if (t.type == '*') result->op = OPERATOR_MUL;
    else if (t.type == '/') result->op = OPERATOR_DIV;
    else if (t.type == '%') result->op = OPERATOR_MOD;
    else assert(0);

    return result;
}

Ast_Binary *make_binary(Ast *left, Ast_Operator *op, Ast *right) {
    Ast_Binary *result = new Ast_Binary();
    result->left  = left;
    result->right = right;
    result->op    = op;
    return result;
}

Ast *Parser::parse_leaf() {
    Token t = get_next_token();

    if (t.type == TOKEN_NUMBER) return make_number(t);
    if (t.type == TOKEN_IDENT)  return make_variable(t);
    else assert(0);
}

Ast *Parser::parse_increasing_precedence(Ast *left, int min_prec) {
    Token next = peek_next_token();

    if (!is_binary_operator(next)) return left;
    eat_token();

    int next_prec = get_precedence(next);

    if (next_prec <= min_prec) {
        return left;
    } else {
        Ast *right = parse_expression(next_prec);
        return make_binary(left, to_operator(next), right);
    }
}

Ast *Parser::parse_expression(int min_prec) {
    Ast *left = parse_leaf();

    while (true) {
        Ast *node = parse_increasing_precedence(left, min_prec);
        if (node == left) break;
        left = node;
    }

    return left;
}
