#ifndef TOKEN_H_
#define TOKEN_H_

#include "grammar.h"

typedef struct token {
  TokenType type;
  const char* str;
  union {
    FunctionKey fnkey;
    OperatorKey opkey;
    double number;
  } lexeme;
} Token;

int tkn_get_precedence(Token token);
int tkn_get_assoc(Token token);

#endif  // TOKEN_H_
