#include "grammar.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>

#include "token.h"

const char* const TokenTypeLUT[] = {
    [TT_LITERAL] = "literal",
    [TT_VARIABLE] = "variable",
    [TT_OPERATOR] = "operator",
    [TT_FUNCTION] = "function",
    [TT_FUNCTION_ARGUMENT] = "function argument",
    [TT_FUNCTION_ARGUMENT_SEPARATOR] = "function argument separator",
    [TT_PARENTHESIS_LEFT] = "parenthesis left",
    [TT_PARENTHESIS_RIGHT] = "parenthesis right",
    [TT_BAD_TOKEN] = "bad token",
};

static inline double to_degrees(double radians) {
  return radians * 180.0 / M_PI;
}
static inline double to_radians(double degrees) {
  return degrees * (M_PI / 180);
}
double calc_sin(double x) { return sin(to_radians(x)); }
double calc_cos(double x) { return cos(to_radians(x)); }
double calc_tan(double x) { return tan(to_radians(x)); }
double calc_cot(double x) { return 1.0 / tan(to_radians(x)); }
double calc_asin(double x) { return asin(x); }
double calc_acos(double x) { return acos(x); }
double calc_atan(double x) { return atan(x); }
double calc_acot(double x) { return atan(1.0 / x); }
double calc_sqrt(double x) { return sqrt(x); }
double calc_log(double x) { return log10(x); }
double calc_ln(double x) { return log(x); }

double fn_unary_apply(FnUnaryPtr fn, double a) { return fn(a); }

const FunctionInfo FunctionLUT[] = {
    [FN_ARC_COSINE] = {"acos", &calc_acos, .arg_count = 1},
    [FN_ARC_COTANGENT] = {"acot", &calc_acot, .arg_count = 1},
    [FN_ARC_SINE] = {"asin", &calc_asin, .arg_count = 1},
    [FN_ARC_TANENGT] = {"atan", &calc_atan, .arg_count = 1},
    [FN_COSINE] = {"cos", &calc_cos, .arg_count = 1},
    [FN_COTANGENT] = {"cot", &calc_cot, .arg_count = 1},
    [FN_LOGARITHM_COMMON] = {"log", &calc_log, .arg_count = 1},
    [FN_LOGARITHM_NATURAL] = {"ln", &calc_ln, .arg_count = 1},
    [FN_SINE] = {"sin", &calc_sin, .arg_count = 1},
    [FN_SQUARE_ROOT] = {"sqrt", &calc_sqrt, .arg_count = 1},
    [FN_TANGENT] = {"tan", &calc_tan, .arg_count = 1},
};
const int FunctionLUT_size = sizeof(FunctionLUT) / sizeof(FunctionLUT[0]);

double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mult(double a, double b) { return a * b; }
double div(double a, double b) { return a / b; }
double neg(double a) { return -a; }

double fn_binary_apply(FnBinaryPtr fn, double a, double b) { return fn(a, b); }

const OperatorInfo OperatorLUT[] = {
    [OP_ADDITION] =
        {
            .lexeme = "+",
            .arg_count = 2,
            .fn_ptr.binary = &add,
            .precedence = 1,
            .assoc = ASSOC_LEFT,
        },
    [OP_SUBTRACTION] =
        {
            .lexeme = "-",
            .arg_count = 2,
            .fn_ptr.binary = &sub,
            .precedence = 1,
            .assoc = ASSOC_LEFT,
        },
    [OP_NEGATIVE] =
        {
            .lexeme = "-",
            .arg_count = 1,
            .fn_ptr.unary = &neg,
            .precedence = 3,
            .assoc = ASSOC_LEFT,
        },
    [OP_MULTIPLICATION] =
        {
            .lexeme = "*",
            .arg_count = 2,
            .fn_ptr.binary = &mult,
            .precedence = 2,
            .assoc = ASSOC_LEFT,
        },
    [OP_DIVISION] =
        {
            .lexeme = "/",
            .arg_count = 2,
            .fn_ptr.binary = &div,
            .precedence = 2,
            .assoc = ASSOC_LEFT,
        },
    [OP_POWER] =
        {
            .lexeme = "^",
            .arg_count = 2,
            .fn_ptr.binary = &pow,
            .precedence = 3,
            .assoc = ASSOC_LEFT,
        },
    [OP_MODULUS] =
        {
            .lexeme = "mod",
            .arg_count = 2,
            .fn_ptr.binary = &fmod,
            .precedence = 2,
            .assoc = ASSOC_LEFT,
        },
};
const int OperatorLUT_size = sizeof(OperatorLUT) / sizeof(OperatorLUT[0]);

FnUnaryPtr fn_unary_getptr(FunctionKey key) {
  assert(FunctionLUT[key].arg_count == 1);
  return FunctionLUT[key].fn_unary_ptr;
}
FnUnaryPtr op_unary_getptr(OperatorKey key) {
  assert(OperatorLUT[key].arg_count == 1);
  return OperatorLUT[key].fn_ptr.unary;
}
FnBinaryPtr op_binary_getptr(OperatorKey key) {
  assert(OperatorLUT[key].arg_count == 2);
  return OperatorLUT[key].fn_ptr.binary;
}

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

Associativity op_get_assoc(OperatorKey key) { return OperatorLUT[key].assoc; }

int tkn_get_precedence(Token token) {
  assert(token.type == TT_OPERATOR);
  return op_get_precedence(token.lexeme.opkey);
}
int tkn_get_assoc(Token token) {
  assert(token.type == TT_OPERATOR);
  return op_get_assoc(token.lexeme.opkey);
}
int tkn_get_argcount(Token token) {
  if (token.type == TT_OPERATOR)
    return OperatorLUT[token.lexeme.opkey].arg_count;
  if (token.type == TT_FUNCTION)
    return FunctionLUT[token.lexeme.fnkey].arg_count;
  assert(false && "wrong token type: " && token.type);
}
FnUnaryPtr tkn_get_fn_unary(Token token) {
  if (token.type == TT_FUNCTION) return fn_unary_getptr(token.lexeme.fnkey);
  if (token.type == TT_OPERATOR) return op_unary_getptr(token.lexeme.opkey);
  assert(false && "wrong token type: " && token.type);
}

FnBinaryPtr tkn_get_fn_binary(Token token) {
  if (token.type == TT_OPERATOR) return op_binary_getptr(token.lexeme.opkey);
  assert(false && "wrong token type: " && token.type);
}
