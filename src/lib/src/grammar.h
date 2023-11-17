#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <string.h>
#define BAD_KEY (-1)

typedef enum TokenType {
  TT_YET_UNDEFINED,
  TT_LITERAL,
  TT_VARIABLE,
  TT_OPERATOR,
  TT_FUNCTION,
  TT_FUNCTION_ARGUMENT,
  TT_FUNCTION_ARGUMENT_SEPARATOR,
  TT_PARENTHESIS_LEFT,
  TT_PARENTHESIS_RIGHT,
  TT_BAD_TOKEN
} TokenType;

typedef enum FunctionKey {
  FN_ARC_COSINE,
  FN_ARC_COTANGENT,
  FN_ARC_SINE,
  FN_ARC_TANENGT,
  FN_COSINE,
  FN_COTANGENT,
  FN_LOGARITHM_COMMON,
  FN_LOGARITHM_NATURAL,
  FN_SINE,
  FN_SQUARE_ROOT,
  FN_TANGENT,
} FunctionKey;

typedef double (*FnUnaryPtr)(double);
typedef struct FunctionInfo {
  const char *const lexeme;
  FnUnaryPtr fn_unary_ptr;
  int arg_count;
} FunctionInfo;

typedef double (*FnUnaryPtr)(double);
typedef enum {
  OP_ADDITION,
  OP_SUBTRACTION,
  OP_NEGATIVE,
  OP_MULTIPLICATION,
  OP_DIVISION,
  OP_POWER,
  OP_MODULUS,
} OperatorKey;
typedef enum {
  ASSOC_LEFT,
  ASSOC_RIGHT,
} Associativity;

typedef double (*FnBinaryPtr)(double, double);
typedef struct OperatorInfo {
  const char *const lexeme;
  int arg_count;
  union {
    FnUnaryPtr unary;
    FnBinaryPtr binary;
  } fn_ptr;
  int precedence;
  Associativity assoc;
} OperatorInfo;

extern const char *const TokenTypeLUT[];
extern const OperatorInfo OperatorLUT[];
extern const FunctionInfo FunctionLUT[];

int fn_get_keyfromstr(const char *const str);
int op_get_keyfromstr(const char *const str);
const char *op_get_strfromkey(OperatorKey key);
const char *fn_get_strfromkey(FunctionKey key);
FnUnaryPtr fn_unary_getptr(FunctionKey key);
FnBinaryPtr op_binary_getptr(OperatorKey key);
double fn_unary_apply(FnUnaryPtr fn, double a);
double fn_binary_apply(FnBinaryPtr fn, double a, double b);

int fn_get_argcount(FunctionKey key);

int op_get_precedence(OperatorKey key);
Associativity op_get_assoc(OperatorKey key);

#endif  // GRAMMAR_H_
