#include <stdio.h>

#include "error_handling.h"
#include "grammar.h"
#include "strutils.h"
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

char* smartcalc_expr_infix_to_postfix(const char* expr) {
  TokenNode* token_list = tokenize(expr);
  ErrorCode error = validate_tokens(token_list);
  if (error) {
    terminate(error);
  }
  TokenQueue* queue = infix_to_postfix(token_list);
  char* res = tkn_queue_tostr(queue);
  return res;
}
