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

void remove_spaces(const char* src, char** res_p) {
  assert(src != NULL);
  assert(res_p != NULL);

  int spaces_count = strcount(src, ' ');
  int res_len = strlen(src) - spaces_count + 1;
  *res_p = calloc(res_len, sizeof(char));
  assert(*res_p != NULL);
  char* res = *res_p;

  *res = *src;
  while (*src != '\0') {
    if (*src != ' ') {
      *res = *src;
      res++;
    }
    src++;
  }
  *res = '\0';
}

bool iscomma(char ch) { return ch == ','; }
bool isparen(char ch) { return ch == '(' || ch == ')'; }
bool isradixp(char ch) { return ch == '.'; }
bool isdigitf(char ch) { return isdigit(ch) || isradixp(ch); }

Token* read_lexeme(const char* src, int* pos) {
  assert(src != NULL);
  assert(pos != NULL);

  int start_idx = *pos;
  const char* ch = &src[*pos];

  int token_len = 1;
  if (isdigit(src[*pos])) {
    int radixp_count = 0;
    while (isdigitf(*ch + token_len)) {
      if (isradixp(*ch + token_len)) radixp_count++;
      if (radixp_count == 2) break;
      token_len++;
    }
  }
  if (isalpha(src[*pos])) {
    while (isalpha(*ch + token_len)) {
      token_len++;
    }
  }

  char* lexem = calloc(token_len + 1, sizeof(char));
  assert(lexem != NULL);
  strncpy(&src[*pos], lexem, token_len);
}

ErrorCode tokenize(char* const src, TokenNode** list_head) {
  assert(src != NULL);
  assert(list_head != NULL);

  char* res;
  remove_spaces(src, &res);
  if (strlen(res) == 0) {
    return E_EMPTY_SOURCE_STRING;
  }

  *list_head = (TokenNode*)malloc(sizeof(TokenNode));
  assert(list_head != NULL);

  TokenNode* token_node = *list_head;
  for (int i = 0; i < strlen(src); i++) {
    int error = read_lexeme(src, &i, &token_node->token);
    if (error) return error;

    token_node->next = malloc(sizeof(TokenNode));
    assert(token_node->next != NULL);
    token_node = token_node->next;
  }

  return E_SUCCESS;
}

int main(void) {
  char expr[] = "1+2-(3*8)";

  TokenNode* token_list = NULL;
  tokenize(expr, &token_list);
  TokenNode* token_node = token_list;
  while (token_node) {
    printf("%s\n", token_node->token.lexeme);
    token_node = token_node->next;
  }
}
