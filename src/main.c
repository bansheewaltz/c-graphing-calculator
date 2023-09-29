#include <stdio.h>

#include "smartcalc.h"
#define INPUT_LEN 256

// add check for function arg separator validity
int main(void) {
  char expr[] = "2 / (3 + 2) * 5";
  smartcalc_expr_analyze(expr);
  char* expr_postfix = smartcalc_expr_infix_to_postfix(expr);
  printf("%s\n", expr_postfix);
}
