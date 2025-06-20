#include "ast.h"

Ast_Expression::Ast_Expression() {
    type = AST_EXPRESSION;
}

Ast_Binary::Ast_Binary() {
    type = AST_BINARY;
}

Ast_Number::Ast_Number() {
    type = AST_NUMBER;
}

Ast_Variable::Ast_Variable() {
    type = AST_VARIABLE;
}

Ast_Operator::Ast_Operator() {
    type = AST_OPERATOR;
}
