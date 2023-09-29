#ifndef TKN_STACK_H_
#define TKN_STACK_H_

#include <stdbool.h>

#include "token.h"

typedef struct TokenStack TokenStack;

TokenStack *tkn_stack_create(int capacity);
void tkn_stack_remove(TokenStack *stack);
bool tkn_stack_isfull(const TokenStack *const stack);
bool tkn_stack_isempty(const TokenStack *const stack);
void tkn_stack_push(TokenStack *const stack, Token token);
Token tkn_stack_pop(TokenStack *const stack);
Token tkn_stack_peek(TokenStack *const stack);

#endif  // TKN_STACK_H_
