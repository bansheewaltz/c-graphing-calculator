#ifndef SMARTCALC_H_
#define SMARTCALC_H_

#include "error_handling.h"

char* smartcalc_expr_infix_to_postfix(const char* expr);
void smartcalc_expr_analyze(const char* expr);
ErrorCode smartcalc_expr_infix_evaluate(const char* expr);
ErrorCode smartcalc_expr_postfix_evaluate(const char* expr);

#endif  // SMARTCALC_H_
