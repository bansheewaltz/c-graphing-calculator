#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_handling.h"
#include "grammar.h"
#include "strutils.h"
#include "tkn_linkedl.h"
#include "token.h"

char* read_lexeme(const char* src, size_t* pos) {
  assert(src != NULL);
  assert(pos != NULL);

  int start_idx = *pos;
  int end_idx = start_idx + 1;

  if (isdigit(src[start_idx])) {
    int radixp_count = 0;
    while (isdigitf(src[end_idx])) {
      if (isradixp(src[end_idx])) radixp_count++;
      if (radixp_count == 2) break;
      end_idx++;
    }
  }
  if (isalpha(src[start_idx])) {
    while (isalpha(src[end_idx])) {
      end_idx++;
    }
  }

  size_t lexeme_len = end_idx - start_idx;
  char* lexeme = strzcreate(lexeme_len);
  strncpy(lexeme, &src[*pos], lexeme_len);
  *pos += lexeme_len;
  return lexeme;
}

bool lxm_isparenl(const char* const lexeme) {
  return lexeme[0] == '(' && strlen(lexeme) == 1;
}
bool lxm_isparenr(const char* const lexeme) {
  return lexeme[0] == ')' && strlen(lexeme) == 1;
}
bool lxm_isfloat_wo_integral_part(const char* const lexeme) {
  return isradixp(lexeme[0]) && strlen(lexeme) > 1;
}
bool lxm_isnumber(const char* const lexeme) {
  return isdigit(lexeme[0]) || lxm_isfloat_wo_integral_part(lexeme);
}
bool lxm_issepart(const char* const lexeme) {
  return lexeme[0] == ',' && strlen(lexeme) == 1;
}
bool lxm_isfunctn(const char* const lexeme) {
  int key = fn_get_keyfromstr(lexeme);
  return key != BAD_KEY;
}
bool lxm_isoperat(const char* const lexeme) {
  int key = op_get_keyfromstr(lexeme);
  return key != BAD_KEY;
}
bool lxm_isvariab(const char* const lexeme) {
  return isalpha(lexeme[0]) && !lxm_isoperat(lexeme) && !lxm_isfunctn(lexeme);
}

TokenType match_token_type(const char* const lexeme) {
  if (lxm_isparenl(lexeme)) return TT_PARENTHESIS_LEFT;
  if (lxm_isparenr(lexeme)) return TT_PARENTHESIS_RIGHT;
  if (lxm_isnumber(lexeme)) return TT_LITERAL;
  if (lxm_issepart(lexeme)) return TT_FUNCTION_ARGUMENT_SEPARATOR;
  if (lxm_isfunctn(lexeme)) return TT_FUNCTION;
  if (lxm_isoperat(lexeme)) return TT_OPERATOR;
  if (lxm_isvariab(lexeme)) return TT_VARIABLE;
  return TT_BAD_TOKEN;
}

Token create_token(const char* const lexeme, TokenType type) {
  Token token = {.type = type, .str = lexeme};
  switch (token.type) {
    case TT_LITERAL:
      token.lexeme.number = atof(lexeme);
      break;
    case TT_FUNCTION:;
      token.lexeme.fnkey = fn_get_keyfromstr(lexeme);
      break;
    case TT_OPERATOR:
      token.lexeme.opkey = op_get_keyfromstr(lexeme);
      break;
    default:
      break;
  }
  return token;
}

Token read_token(const char* const src, size_t* pos) {
  const char* const lexeme = read_lexeme(src, pos);
  TokenType type = match_token_type(lexeme);
  Token token = create_token(lexeme, type);
  return token;
}

TokenNode* tokenize(const char* src) {
  assert(src != NULL);
  src = strremove(src, ' ');
  if (strisempty(src)) {
    return NULL;
  }
  size_t pos = 0;
  Token token = read_token(src, &pos);
  TokenNode* list_head = tkn_linkedl_createnode(token);
  TokenNode* token_node = list_head;
  while (pos < strlen(src)) {
    token = read_token(src, &pos);
    token_node->next = tkn_linkedl_createnode(token);
    token_node = token_node->next;
  }
  return list_head;
}

ErrorCode validate_tokens(const TokenNode* const list_head) {
  if (list_head == NULL) {
    return E_EMPTY_SOURCE_STRING;
  }
  int parentheses_balance = 0;
  bool term_presence = false;
  const TokenNode* node = list_head;
  while (node) {
    Token token = node->token;
    if (token.type == TT_BAD_TOKEN) return E_BAD_TOKEN;
    if (token.type == TT_LITERAL || token.type == TT_VARIABLE)
      term_presence = true;
    if (token.type == TT_PARENTHESIS_LEFT) parentheses_balance++;
    if (token.type == TT_PARENTHESIS_RIGHT) parentheses_balance--;
    if (parentheses_balance < 0) return E_PARENTHESES_INVALID_SEQUENCE;
    node = node->next;
  }
  if (parentheses_balance != 0) return E_PARENTHESES_NOT_BALANCED;
  if (term_presence == false) return E_EMPTY_EXPRESSION;
  return E_SUCCESS;
}
