#ifndef SMARTCALC_H_
#define SMARTCALC_H_

typedef enum ErrorCode {
  SMARTCALC_SUCCESS,
  SMARTCALC_EMPTY_SOURCE_STRING,
  SMARTCALC_BAD_TOKEN,
  SMARTCALC_INVALID_PARENTHESES_SEQUENCE,
  SMARTCALC_EMPTY_EXPRESSION,
  SMARTCALC_SEPARATOR_OUT_OF_CONTEXT,
} SmartCalcError;

#ifdef __cplusplus
extern "C" {
#endif

void smartcalc_expr_analyze(const char* expr);
char* smartcalc_expr_infix_to_postfix(const char* expr);
double smartcalc_expr_infix_evaluate(const char* expr);

const char* smartcalc_error_get_description(SmartCalcError error);
void smartcalc_error_terminate(SmartCalcError error);

#ifdef __cplusplus
}
#endif

#endif  // SMARTCALC_H_
