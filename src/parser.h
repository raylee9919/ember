#pragma once

#include "types.h"
#include "lexer.h"
#include "ast.h"

const int MIN_PREC = -1000;

struct Parser {
    void parse();
    Token peek_next_token();
    void eat_token();
    Token get_next_token();
    Ast *parse_leaf();
    Ast *parse_increasing_precedence(Ast *left, int min_prec);
    Ast *parse_expression(int min_prec);

    Lexer *lexer;

    bool input_ended;
};
