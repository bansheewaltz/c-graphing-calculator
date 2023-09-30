#include <assert.h>
#include <stdlib.h>

#include "tkn_queuearr.h"
#include "token.h"

typedef struct {
  int capacity;
  int top;
  double *arr;
} NumberStack;

static NumberStack *num_stack_create(size_t capacity) {
  NumberStack *stack = malloc(sizeof(NumberStack));
  assert(stack != NULL);
  stack->arr = malloc(capacity * sizeof(double));
  assert(stack->arr != NULL);
  stack->capacity = capacity;
  stack->top = -1;
  return stack;
}
static bool num_stack_isfull(const NumberStack *stack) {
  assert(stack != NULL);
  return stack->top == stack->capacity - 1;
}
static bool num_stack_isempty(const NumberStack *stack) {
  assert(stack != NULL);
  return stack->top == -1;
}
static void num_stack_push(NumberStack *stack, double value) {
  assert(stack != NULL);
  assert(!num_stack_isfull(stack));
  stack->top++;
  stack->arr[stack->top] = value;
}
static double num_stack_pop(NumberStack *stack) {
  assert(stack != NULL);
  assert(!num_stack_isempty(stack));
  return stack->arr[stack->top--];
}

double tkn_queue_postfix_evaluate(TokenQueue *queue) {
  size_t queue_size = tkn_queue_getsize(queue);
  NumberStack *num_stack = num_stack_create(queue_size);
  while (!tkn_queue_isempty(queue)) {
    Token token = tkn_queue_dequeue(queue);
    switch (token.type) {
      case TT_LITERAL:
        num_stack_push(num_stack, token.lexeme.number);
        break;
      case TT_OPERATOR: {
        double rigth_num = num_stack_pop(num_stack);
        double left_num = num_stack_pop(num_stack);
        FnBinaryPtr operation = op_binary_getptr(token.lexeme.opkey);
        double res = fn_binary_apply(operation, left_num, rigth_num);
        num_stack_push(num_stack, res);
        break;
      }
      case TT_FUNCTION:
        if (tkn_get_argcount(token) == 1) {
          double num = num_stack_pop(num_stack);
          FnUnaryPtr operation = fn_unary_getptr(token.lexeme.fnkey);
          double res = fn_unary_apply(operation, num);
          num_stack_push(num_stack, res);
        }
        break;
      default:
        assert(!token.type);
    }
  }
  return num_stack_pop(num_stack);
}