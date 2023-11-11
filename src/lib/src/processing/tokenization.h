#ifndef TOKENIZATION_H_
#define TOKENIZATION_H_

#include "grammar.h"
#include "tkn_queuearr.h"

TokenType match_token_type(char *lexeme);
Token create_token(const char *lexeme, TokenType type);
Token read_token(const char *src, size_t *pos);
TokenNode *tokenize(const char *src);
SmartCalcError validate_tokens(const TokenNode *const list_head);

#endif  // TOKENIZATION_H_
