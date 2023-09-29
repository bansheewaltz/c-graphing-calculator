
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "token.h"

#define EMPTY -1

typedef struct TokenStack {
  int capacity;
  Token* array;
  int top;
} TokenStack;

TokenStack* tkn_stack_create(int capacity) {
  TokenStack* stack = malloc(sizeof(TokenStack) * capacity);
  assert(stack != NULL);
  stack->array = malloc(sizeof(Token) * capacity);
  assert(stack->array != NULL);
  stack->capacity = capacity;
  stack->top = EMPTY;
  return stack;
}

void tkn_stack_remove(TokenStack* stack) {
  if (stack == NULL) return;
  if (stack->array) {
    free(stack->array);
  }
  free(stack);
}

bool tkn_stack_isfull(const TokenStack* const stack) {
  assert(stack != NULL);
  return stack->top == stack->capacity - 1;
}

bool tkn_stack_isempty(const TokenStack* const stack) {
  assert(stack != NULL);
  return stack->top == EMPTY;
}

void tkn_stack_push(TokenStack* const stack, Token token) {
  assert(stack != NULL);
  assert(!tkn_stack_isfull(stack));
  stack->top++;
  stack->array[stack->top] = token;
}

Token tkn_stack_pop(TokenStack* const stack) {
  assert(stack != NULL);
  assert(!tkn_stack_isempty(stack));
  return stack->array[stack->top--];
}

Token tkn_stack_peek(TokenStack* const stack) {
  assert(stack != NULL);
  assert(!tkn_stack_isempty(stack));
  return stack->array[stack->top];
}
