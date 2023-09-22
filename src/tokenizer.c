#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum error_code {
  E_SUCCESS,
  E_NULL_POINTER_INPUT,
  E_MEMORY_ALLOCATION_FAILURE,
  E_EMPTY_SOURCE_STRING,
  E_INCORRECT_TOKEN,
} ErrorCode;

typedef enum token_type {
  LITERAL,
  VARIABLE,
  OPERATOR,
  UNARY_OPERATOR,
  FUNCTION,
  FUNCTION_ARGUMENT,
  FUNCTION_ARGUMENT_SEPARATOR,
  PARANTHESIS_LEFT,
  PARANTHESIS_RIGHT
} TokenType;

typedef char* str;
typedef struct token {
  TokenType type;
  str lexeme;
} Token;

typedef struct token_node {
  Token token;
  struct token_node* next;
} TokenNode;

int strcount(const char* str, char ch) {
  assert(str != NULL);
  int count = 0;
  for (int i = 0; i < strlen(str); i++) {
    if (isspace(str[i])) count++;
  }
  return count;
}

char* remove_spaces(const char* src) {
  assert(src != NULL);
  int spaces_count = strcount(src, ' ');
  int res_len = strlen(src) - spaces_count + 1;
  char* res = calloc(res_len, sizeof(char));
  *res = *src;
  while (*src != '\0') {
    if (*src != ' ') {
      *res = *src;
      res++;
    }
    src++;
  }
  *res = '\0';
  return res;
}

bool iscomma(char ch) { return ch == ','; }
bool isparen(char ch) { return ch == '(' || ch == ')'; }
bool isradixp(char ch) { return ch == '.'; }
bool isdigitf(char ch) { return isdigit(ch) || isradixp(ch); }

char* read_lexeme(const char* src, int* pos) {
  assert(src != NULL);
  assert(pos != NULL);

  int start_idx = *pos;
  const char* ch = &src[*pos];

  int lexeme_len = 1;
  if (isdigit(src[start_idx])) {
    int radixp_count = 0;
    while (isdigitf(*ch + lexeme_len)) {
      if (isradixp(*ch + lexeme_len)) radixp_count++;
      if (radixp_count == 2) break;
      lexeme_len++;
    }
  }
  if (isalpha(src[start_idx])) {
    while (isalpha(*ch + lexeme_len)) {
      lexeme_len++;
    }
  }

  char* lexeme = calloc(lexeme_len + 1, sizeof(char));
  assert(lexeme != NULL);
  strncpy(&src[*pos], lexeme, lexeme_len);
  *pos += lexeme_len;
  return lexeme;
}

TokenType match_token_type(str lexeme) {
  ;
  ;
}

Token create_token(str lexeme, TokenType type) {
  Token token;
  token.lexeme = lexeme;
  token.type = type;
  return token;
}

Token read_token(const char* src, int* pos) {
  char* lexeme = read_lexeme(src, pos);
  TokenType type = match_token_type(lexeme);
  Token token = create_token(lexeme, type);
  return token;
}

TokenNode* create_token_node(Token token) {
  TokenNode* node = malloc(sizeof(TokenNode));
  node->token = token;
  node->next = NULL;
  return node;
}

TokenNode* tokenize(char* const src) {
  assert(src != NULL);
  char* src_wo_spaces = remove_spaces(src);
  if (strlen(src_wo_spaces) == 0) {
    return NULL;
  }
  int pos = 0;
  Token token = read_token(src, &pos);
  TokenNode* token_node = create_token_node(token);
  while (token_node != NULL) {
    token = read_token(src, &pos);
    token_node->next = create_token_node(token);
    token_node = token_node->next;
  }
  return token_node;
}

int main(void) {
  char expr[] = "1+2-(3*8)";

  TokenNode* token_list = tokenize(expr);
  TokenNode* token_node = token_list;
  while (token_node) {
    printf("%s\n", token_node->token.lexeme);
    token_node = token_node->next;
  }
}
