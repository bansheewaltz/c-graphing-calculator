#include <stdio.h>

#include "infix_to_postfix.h"
#include "postfix_evaluation.h"
#include "smartcalc.h"
#include "tkn_linkedl.h"
#include "tkn_queuearr.h"
#include "tokenization.h"

static void print_src_string(const char* const src) {
  printf("Source string: \"%s\"\n", src);
}

/**
 * The function smartcalc_expr_analyze analyzes an expression by tokenizing it
 * and printing the source string and the token list.
 *
 * @param expr The `expr` parameter is a pointer to a constant character array,
 * which represents the expression that needs to be analyzed.
 */
void smartcalc_expr_analyze(const char* const expr) {
  TokenNode* token_list = tokenize(expr);
  print_src_string(expr);
  tkn_linkedl_print(token_list);
}

/**
 * The function converts an infix expression to postfix notation and returns the
 * result as a string.
 *
 * @param expr The `expr` parameter is a pointer to a character array that
 * represents an infix mathematical expression.
 * @param result The "result" parameter is a pointer to a pointer to a character
 * array. It is used to store the resulting postfix expression as a string.
 *
 * @return a SmartCalcError value.
 */
SmartCalcError smartcalc_expr_infix_to_postfix(const char* expr,
                                               char** result) {
  TokenNode* token_list = tokenize(expr);
  SmartCalcError rc = validate_tokens(token_list);
  if (rc) {
    return rc;
  }
  TokenQueue* queue = infix_to_postfix(token_list);
  *result = tkn_queue_tostr(queue);
  return SMARTCALC_ERR_SUCCESS;
}

/**
 * The function evaluates an infix mathematical expression with a variable value
 * and returns the result.
 *
 * @param expr A string representing a mathematical expression in infix
 * notation.
 * @param x The parameter `x` is a double value that represents the value of the
 * variable `x` in the expression. It is used to substitute the variable `x` in
 * the expression before evaluating it.
 * @param result A pointer to a double variable where the result of the
 * expression evaluation will be stored.
 *
 * @return a value of type SmartCalcError.
 */
SmartCalcError smartcalc_expr_infix_evaluate(const char* expr, double x,
                                             double* result) {
  TokenNode* token_list = tokenize(expr);
  tkn_linkedl_insert_var_value(token_list, x);
  SmartCalcError rc = validate_tokens(token_list);
  if (rc) {
    return rc;
  }
  TokenQueue* queue = infix_to_postfix(token_list);
  *result = tkn_queue_postfix_evaluate(queue);
  return SMARTCALC_ERR_SUCCESS;
}
