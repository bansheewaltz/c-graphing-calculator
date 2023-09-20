#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum token_type {
  NUMBER,
  OPERATOR,
  FUNCTION,
  PARANTHESIS_LEFT,
  PARANTHESIS_RIGHT
} TokenType;

typedef enum operator{
  PLUS,
  MINUS,
} Operator;

typedef enum function {
  SIN,
  COS,
  TAN,
  COT,
  ASIN,
  ACOS,
  ATAN,
  ACOT,
  SQRT,
  LN,
  LOG
} Function;

typedef enum operator_precedence {
  SIN = 1,
  COS = 2,
  TAN = 3,
  COT = 4,
  ASIN = 5,
  ACOS = 6,
  ATAN = 7,
  ACOT = 8,
  SQRT = 9,
  LN = 10,
  LOG = 11
} Precedence;

// typedef struct {
//   char* key;
//   int val;
// } t_symstruct;

// static t_symstruct lookuptable[] = {
//     {"A1", A1}, {"A2", A2}, {"B1", B1}, {"B2", B2}};

// int keyfromstring(char* key) {
//   int i;
//   for (i = 0; i < NKEYS; i++) {
//     t_symstruct* sym = lookuptable[i];
//     if (strcmp(sym->key, key) == 0) return sym->val;
//   }
//   return BADKEY;
// }

typedef struct token {
  TokenType type;
  union {
    double value;
    Function function;
    Operator operator;
  };
} Token;

typedef struct token_node {
  Token token;
  struct token_node* next;
} TokenNode;

// typedef char* str;

typedef enum status_code {
  E_SUCCESS,
  E_MEMORY_ALLOCATION_FAILURE,
  E_INCORRECT_TOKEN,
} ErrorCode;

// int evaluate(char* const expr, double* result) {
//   ReturnCode error;
//   int i = 0;
//   while (expr[i] != '\0') {
//     Token token;
//     error = read_token(expr, &i, &token);
//     if (error) {
//       // return
//     }
//     if (token.type ==) }
// }

// new_node static()

double parse_number(char const* str, int* pos) {
  while (isdigit(str[*pos] ||))
    ;
}

bool static is_operator(char const* str, int* pos) {
  char ch = str[*pos];
  switch (ch) {
    case '+':
      return true;
    case '-':
      return true;
    case '/':
      return true;
    case '*':
      return true;
    case 'm':
      return true;
    default:
      return false;
  }
}

ErrorCode read_token(char* const str, int* pos, Token* token) {
  char ch = str[*pos];
  if (isdigit(ch)) {
    double val = parse_number(str, pos);
    token->type = NUMBER;
    return E_SUCCESS;
  }
  if (isoperator(str, pos)) {
    token->type = OPERATOR;
    return E_SUCCESS;
  }
  if (isfunction(str, pos)) {
    token->type = OPERATOR;
    return E_SUCCESS;
  }
  switch (ch) {
    case ' ':
      break;
    case '(':
      token->type = PARANTHESIS_LEFT;
      break;
    case ')':
      token->type = PARANTHESIS_RIGHT;
      break;
    default:
      return E_INCORRECT_TOKEN;
  }
  *pos = *pos + 1;
  return E_SUCCESS;
}

ErrorCode tokenize(char* const expr, TokenNode** list_head) {
  *list_head = (TokenNode*)malloc(sizeof(TokenNode));
  if (list_head == NULL) {
    return E_MEMORY_ALLOCATION_FAILURE;
  }
  TokenNode* token_node = *list_head;
  for (int i = 0; i < strlen(expr); i++) {
    int error = read_token(expr, &i, &token_node->token);
    if (error) return error;
    token_node->next = malloc(sizeof(TokenNode));
    if (token_node == NULL) {
      return E_MEMORY_ALLOCATION_FAILURE;
    }
    token_node = token_node->next;
  }
  return E_SUCCESS;
}

int main(void) {
  char expr[] = "1+2-(3*8)";
  TokenNode* token_list;
  int error = tokenize(expr, &token_list);
  if (error) return error;

  TokenNode* token_node = token_list;
  do {
    printf("%d", token_node->token.type);
    token_node = token_node->next;
  } while (token_node);
}

// struct _errordesc {
//   int code;
//   char* message;
// } errordesc[] = {
//     {E_SUCCESS, "No error"},
//     {E_INVALID_INPUT, "Invalid input"},
//     {E_FILE_NOT_FOUND, "File not found"},
// };
