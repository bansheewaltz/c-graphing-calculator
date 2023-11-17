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

void smartcalc_expr_analyze(const char* const expr) {
  TokenNode* token_list = tokenize(expr);
  print_src_string(expr);
  tkn_linkedl_print(token_list);
}

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
