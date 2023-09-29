#include "grammar.h"

#include <assert.h>

#include "token.h"

const char* const TokenTypeLUT[] = {
    [TT_LITERAL] = "literal",
    [TT_VARIABLE] = "variable",
    [TT_OPERATOR] = "operator",
    [TT_UNARY_OPERATOR] = "unary operator",
    [TT_FUNCTION] = "function",
    [TT_FUNCTION_ARGUMENT] = "function argument",
    [TT_FUNCTION_ARGUMENT_SEPARATOR] = "function argument separator",
    [TT_PARENTHESIS_LEFT] = "parenthesis left",
    [TT_PARENTHESIS_RIGHT] = "parenthesis right",
    [TT_BAD_TOKEN] = "bad token",
};

const FunctionInfo FunctionLUT[] = {
    [FN_ARC_COSINE] = {"acos", 1},
    [FN_ARC_COTANGENT] = {"acot", 1},
    [FN_ARC_SINE] = {"asin", 1},
    [FN_ARC_TANENGT] = {"atan", 1},
    [FN_COSINE] = {"cos", 1},
    [FN_COTANGENT] = {"cot", 1},
    [FN_LOGARITHM_COMMON] = {"log", 1},
    [FN_LOGARITHM_NATURAL] = {"ln", 1},
    [FN_SINE] = {"sin", 1},
    [FN_SQUARE_ROOT] = {"sqrt", 1},
    [FN_TANGENT] = {"tan", 1},
};
const int FunctionLUT_size = sizeof(FunctionLUT) / sizeof(FunctionLUT[0]);

const OperatorInfo OperatorLUT[] = {
    [OP_ADDITION] = {"+", .precedence = 1, ASSOC_LEFT},
    [OP_SUBTRACTION] = {"-", .precedence = 1, ASSOC_LEFT},
    [OP_MULTIPLICATION] = {"*", .precedence = 2, ASSOC_LEFT},
    [OP_DIVISION] = {"/", .precedence = 2, ASSOC_LEFT},
    [OP_POWER] = {"^", .precedence = 3, ASSOC_LEFT},
    [OP_MODULUS] = {"mod", .precedence = 2, ASSOC_LEFT},
};
const int OperatorLUT_size = sizeof(OperatorLUT) / sizeof(OperatorLUT[0]);

int fn_get_keyfromstr(const char* const str) {
  for (int i = 0; i < FunctionLUT_size; i++) {
    if (strcmp(FunctionLUT[i].lexeme, str) == 0) {
      return i;
    }
  }
  return BAD_KEY;
}

const char* fn_get_strfromkey(FunctionKey key) {
  if ((int)key < 0 || (int)key >= FunctionLUT_size) {
    return NULL;
  }
  return FunctionLUT[key].lexeme;
}

int fn_get_argcount(FunctionKey key) { return FunctionLUT[key].arg_count; }

int op_get_keyfromstr(const char* const str) {
  for (int i = 0; i < OperatorLUT_size; i++) {
    if (strcmp(OperatorLUT[i].lexeme, str) == 0) {
      return i;
    }
  }
  return BAD_KEY;
}

const char* op_get_strfromkey(OperatorKey key) {
  if ((int)key < 0 || (int)key >= OperatorLUT_size) {
    return NULL;
  }
  return OperatorLUT[key].lexeme;
}

int op_get_precedence(OperatorKey key) { return OperatorLUT[key].precedence; }
int tkn_get_precedence(Token token) {
  assert(token.type == TT_OPERATOR);
  return op_get_precedence(token.lexeme.opkey);
}

Associativity op_get_assoc(OperatorKey key) { return OperatorLUT[key].assoc; }
int tkn_get_assoc(Token token) {
  assert(token.type == TT_OPERATOR);
  return op_get_assoc(token.lexeme.opkey);
}
