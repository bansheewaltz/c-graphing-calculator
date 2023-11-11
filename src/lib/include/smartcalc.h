#ifndef SMARTCALC_H_
#define SMARTCALC_H_

typedef enum ErrorCode {
  E_SUCCESS,
  E_EMPTY_SOURCE_STRING,
  E_BAD_TOKEN,
  E_PARENTHESES_INVALID_SEQUENCE,
  E_PARENTHESES_NOT_BALANCED,
  E_EMPTY_EXPRESSION,
  E_SEPARATOR_OUT_OF_CONTEXT,
} ErrorCode;

void smartcalc_expr_analyze(const char* expr);
char* smartcalc_expr_infix_to_postfix(const char* expr);
double smartcalc_expr_infix_evaluate(const char* expr);

const char* smartcalc_error_get_description(ErrorCode error);
void smartcalc_error_terminate(ErrorCode error);

#endif  // SMARTCALC_H_
