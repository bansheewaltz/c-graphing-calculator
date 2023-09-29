#ifndef TKN_LINKEDL_H_
#define TKN_LINKEDL_H_

#include "token.h"

typedef struct token_node {
  Token token;
  struct token_node *next;
} TokenNode;

TokenNode *tkn_linkedl_createnode(Token token);
void tkn_linkedl_print(const TokenNode *const list_head);
int tkn_linkedl_getlength(const TokenNode *const list_head);

#endif  // TKN_LINKEDL_H_
