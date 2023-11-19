#ifndef SMARTCALC_H_
#define SMARTCALC_H_

/** @file smartcalc.h */

#define SMARTCALC_INPUT_MAX_LEN 256

typedef enum ErrorCode {
  SMARTCALC_ERR_SUCCESS,
  SMARTCALC_ERR_EMPTY_SOURCE_STRING,
  SMARTCALC_ERR_BAD_TOKEN,
  SMARTCALC_ERR_INVALID_PARENTHESES_SEQUENCE,
  SMARTCALC_ERR_EMPTY_EXPRESSION,
  SMARTCALC_ERR_SEPARATOR_OUT_OF_CONTEXT,
} SmartCalcError;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The function smartcalc_expr_analyze analyzes an expression by tokenizing it
 * and printing the source string and the token list.
 *
 * @param expr The `expr` parameter is a pointer to a constant character array,
 * which represents the expression that needs to be analyzed.
 */
void smartcalc_expr_analyze(const char* expr);

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
SmartCalcError smartcalc_expr_infix_to_postfix(const char* expr, char** result);

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
                                             double* result);

/**
 * The function returns the description of a given error code.
 *
 * @param error The parameter "error" is of type SmartCalcError, which is an
 * enumeration representing different error codes in the smart calculator
 * program.
 *
 * @return a pointer to a character array (const char*) which represents the
 * description of the given SmartCalcError.
 */
const char* smartcalc_error_get_description(SmartCalcError error);

#ifdef __cplusplus
}
#endif

#endif  // SMARTCALC_H_
