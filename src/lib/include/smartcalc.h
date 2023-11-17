#ifndef SMARTCALC_H_
#define SMARTCALC_H_

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

void smartcalc_expr_analyze(const char* expr);
// void smartcalc_expr_to_internal_syntax(const char* expr, const char* result);
SmartCalcError smartcalc_expr_infix_to_postfix(const char* expr, char** result);
SmartCalcError smartcalc_expr_infix_evaluate(const char* expr, double x,
                                             double* result);

const char* smartcalc_error_get_description(SmartCalcError error);

#ifdef __cplusplus
}
#endif

#endif  // SMARTCALC_H_
