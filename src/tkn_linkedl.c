#include "tkn_linkedl.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "token.h"

TokenNode* tkn_linkedl_createnode(Token token) {
  TokenNode* node = malloc(sizeof(TokenNode));
  assert(node != NULL);
  node->token = token;
  node->next = NULL;
  return node;
}

static void print_token(Token token) {
  const char* type = TokenTypeLUT[token.type];
  printf("Type: \"%s\", Value: \"%s\"\n", type, token.str);
}

void tkn_linkedl_print(const TokenNode* const list_head) {
  const TokenNode* node = list_head;
  while (node) {
    Token token = node->token;
    print_token(token);
    node = node->next;
  }
}

int tkn_linkedl_getlength(const TokenNode* const list_head) {
  int count = 0;
  const TokenNode* node = list_head;
  while (node) {
    count++;
    node = node->next;
  }
  return count;
}
