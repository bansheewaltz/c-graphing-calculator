#include <assert.h>
#include <stdlib.h>

#include "tkn_linkedl.h"
#include "tkn_queuearr.h"

#define STACK_EMPTY -1

typedef struct TokenStack {
  int capacity;
  Token* array;
  int top;
} TokenStack;

static TokenStack* tkn_stack_create(int capacity) {
  TokenStack* stack = malloc(sizeof(TokenStack) * capacity);
  assert(stack != NULL);
  stack->array = malloc(sizeof(Token) * capacity);
  assert(stack->array != NULL);
  stack->capacity = capacity;
  stack->top = STACK_EMPTY;
  return stack;
}
static void tkn_stack_remove(TokenStack* stack) {
  if (stack == NULL) return;
  if (stack->array) {
    free(stack->array);
  }
  free(stack);
}
static bool tkn_stack_isfull(const TokenStack* const stack) {
  assert(stack != NULL);
  return stack->top == stack->capacity - 1;
}
static bool tkn_stack_isempty(const TokenStack* const stack) {
  assert(stack != NULL);
  return stack->top == STACK_EMPTY;
}
static void tkn_stack_push(TokenStack* const stack, Token token) {
  assert(stack != NULL);
  assert(!tkn_stack_isfull(stack));
  stack->top++;
  stack->array[stack->top] = token;
}
static Token tkn_stack_pop(TokenStack* const stack) {
  assert(stack != NULL);
  assert(!tkn_stack_isempty(stack));
  return stack->array[stack->top--];
}
static Token tkn_stack_peek(TokenStack* const stack) {
  assert(stack != NULL);
  assert(!tkn_stack_isempty(stack));
  return stack->array[stack->top];
}

/**
 * @brief Dijkstra's Shunting yard algorithm
 *
 * @param list_head
 * @return TokenQueue*
 */
TokenQueue* infix_to_postfix(const TokenNode* const list_head) {
  int token_count = tkn_linkedl_getlength(list_head);
  TokenStack* op_stack = tkn_stack_create(token_count);
  TokenQueue* out_queue = tkn_queue_create(token_count);

  const TokenNode* node = list_head;
  while (node) {
    Token token = node->token;
    switch (token.type) {
      case TT_LITERAL:
      case TT_VARIABLE:
      case TT_FUNCTION_ARGUMENT:
        tkn_queue_enqueue(out_queue, token);
        break;
      case TT_FUNCTION:
      case TT_PARENTHESIS_LEFT:
        tkn_stack_push(op_stack, token);
        break;
      case TT_FUNCTION_ARGUMENT_SEPARATOR:
        while (!tkn_stack_isempty(op_stack) &&
               tkn_stack_peek(op_stack).type != TT_PARENTHESIS_LEFT) {
          Token operator= tkn_stack_pop(op_stack);
          tkn_queue_enqueue(out_queue, operator);
        }
        assert(!tkn_stack_isempty(op_stack) &&
               tkn_stack_peek(op_stack).type == TT_PARENTHESIS_LEFT);
        break;
      case TT_OPERATOR:
        while (!tkn_stack_isempty(op_stack) &&
               tkn_stack_peek(op_stack).type == TT_OPERATOR) {
          Token t1 = token;
          Token t2 = tkn_stack_peek(op_stack);
          int prec1 = tkn_get_precedence(t1);
          int prec2 = tkn_get_precedence(t2);
          Associativity assoc1 = tkn_get_assoc(t1);
          if (prec2 > prec1 || (prec1 == prec2 && assoc1 == ASSOC_LEFT)) {
            Token operator2 = tkn_stack_pop(op_stack);
            tkn_queue_enqueue(out_queue, operator2);
          } else {
            break;
          }
        }
        tkn_stack_push(op_stack, token);
        break;
      case TT_PARENTHESIS_RIGHT:
        while (!tkn_stack_isempty(op_stack) &&
               tkn_stack_peek(op_stack).type != TT_PARENTHESIS_LEFT) {
          Token operator= tkn_stack_pop(op_stack);
          tkn_queue_enqueue(out_queue, operator);
        }
        assert(!tkn_stack_isempty(op_stack) &&
               tkn_stack_peek(op_stack).type == TT_PARENTHESIS_LEFT);
        (void)tkn_stack_pop(op_stack);
        if (tkn_stack_peek(op_stack).type == TT_FUNCTION) {
          Token function = tkn_stack_pop(op_stack);
          tkn_queue_enqueue(out_queue, function);
        }
        break;
      default:
        assert(!token.type);
    }
    node = node->next;
  }
  while (!tkn_stack_isempty(op_stack)) {
    assert(tkn_stack_peek(op_stack).type != TT_PARENTHESIS_LEFT);
    Token operator= tkn_stack_pop(op_stack);
    tkn_queue_enqueue(out_queue, operator);
  }
  return out_queue;
}
