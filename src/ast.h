#pragma once

#include "types.h"

enum Ast_Type {
    AST_EXPRESSION,
    AST_BINARY,
    AST_NUMBER,
    AST_VARIABLE,
    AST_OPERATOR,
};

struct Ast {
    Ast_Type type;
};

struct Ast_Expression : public Ast {
    Ast_Expression();
};

enum Operation_Type {
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MUL,
    OPERATOR_DIV,
    OPERATOR_MOD,
};

struct Ast_Operator: public Ast {
    Ast_Operator();
    Operation_Type op;
};

struct Ast_Binary: public Ast_Expression {
    Ast_Binary();

    Ast_Operator *op;
    Ast *left;
    Ast *right;
}; 

struct Ast_Number : public Ast {
    Ast_Number();

    union {
        u64 integer_value;
        f32 float32_value;
        f64 float64_value;
    };
};

struct Ast_Variable : public Ast {
    Ast_Variable();
};
