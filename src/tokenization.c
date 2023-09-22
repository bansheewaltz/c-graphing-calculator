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

#define BAD_KEY -1
typedef struct key_value_pair {
  char* value;
  int key;
} KeyValuePair;

int keyfromstring(char* value, const KeyValuePair lut[], int lut_size) {
  for (int i = 0; i < lut_size; i++) {
    KeyValuePair entry = lut[i];
    if (strcmp(entry.value, value) == 0) {
      return entry.key;
    }
  }
  return BAD_KEY;
}

char* stringfromkey(int key, const KeyValuePair lut[], int lut_size) {
  for (int i = 0; i < lut_size; i++) {
    KeyValuePair entry = lut[i];
    if (entry.key == key) {
      return entry.value;
    }
  }
  return NULL;
}
typedef enum token_type {
  YET_UNDEFINED,
  LITERAL,
  VARIABLE,
  OPERATOR,
  UNARY_OPERATOR,
  FUNCTION,
  FUNCTION_ARGUMENT,
  FUNCTION_ARGUMENT_SEPARATOR,
  PARENTHESIS_LEFT,
  PARENTHESIS_RIGHT,
  BAD_TOKEN
} TokenType;
const KeyValuePair TokenLUT[] = {
    {"literal", LITERAL},
    {"variable", VARIABLE},
    {"operator", OPERATOR},
    {"unary operator", UNARY_OPERATOR},
    {"function", FUNCTION},
    {"function argument", FUNCTION_ARGUMENT},
    {"function argument separator", FUNCTION_ARGUMENT_SEPARATOR},
    {"parenthesis left", PARENTHESIS_LEFT},
    {"parenthesis right", PARENTHESIS_RIGHT},
    {"bad token", BAD_TOKEN},
};
const int TokenLUT_size = sizeof(TokenLUT) / sizeof(TokenLUT[0]);

typedef enum operator{
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  POWER,
  MODULUS,
} Operator;
const KeyValuePair OperatorLUT[] = {
    {"+", ADDITION}, {"-", SUBTRACTION}, {"*", MULTIPLICATION},
    {"/", DIVISION}, {"^", POWER},       {"mod", MODULUS},
};
const int OperatorLUT_size = sizeof(OperatorLUT) / sizeof(OperatorLUT[0]);
typedef enum function {
  ARC_COSINE,
  ARC_COTANGENT,
  ARC_SINE,
  ARC_TANENGT,
  COSINE,
  COTANGENT,
  LOGARITHM_COMMON,
  LOGARITHM_NATURAL,
  SINE,
  SQUARE_ROOT,
  TANGENT,
} Function;
const KeyValuePair FunctionLUT[] = {
    {"sin", SINE},
    {"cos", COSINE},
    {"tan", TANGENT},
    {"cot", COTANGENT},
    {"asin", ARC_SINE},
    {"acos", ARC_COSINE},
    {"atan", ARC_TANENGT},
    {"acot", ARC_COTANGENT},
    {"sqrt", SQUARE_ROOT},
    {"ln", LOGARITHM_NATURAL},
    {"log", LOGARITHM_COMMON},
};
const int FunctionLUT_size = sizeof(FunctionLUT) / sizeof(FunctionLUT[0]);

typedef struct token {
  TokenType type;
  const char* lexeme;
} Token;

typedef struct token_node {
  Token token;
  struct token_node* next;
} TokenNode;

int strcount(const char* str, char ch) {
  assert(str != NULL);
  int count = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == ch) count++;
  }
  return count;
}

char* remove_spaces(const char* src) {
  assert(src != NULL);
  int spaces_count = strcount(src, ' ');
  int res_len = strlen(src) - spaces_count + 1;
  char* res = calloc(res_len, sizeof(char));
  char* iter = res;
  while (*src != '\0') {
    if (*src != ' ') {
      *iter = *src;
      iter++;
    }
    src++;
  }
  *iter = '\0';
  return res;
}

bool iscomma(char ch) { return ch == ','; }
bool isparen(char ch) { return ch == '(' || ch == ')'; }
bool isradixp(char ch) { return ch == '.'; }
bool isdigitf(char ch) { return isdigit(ch) || isradixp(ch); }

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

  int lexeme_len = end_idx - start_idx;
  char* lexeme = calloc(lexeme_len + 1, sizeof(char));
  assert(lexeme != NULL);
  strncpy(lexeme, &src[*pos], lexeme_len);
  *pos += lexeme_len;
  return lexeme;
}

bool l_isvariab(char* lexeme) { return isalpha(lexeme[0]); }
bool l_isparenl(char* lexeme) { return lexeme[0] == '('; }
bool l_isparenr(char* lexeme) { return lexeme[0] == ')'; }

bool l_isfloat_wo_integral_part(char* lexeme) {
  return isradixp(lexeme[0]) && strlen(lexeme) > 1;
}
bool l_isnumber(char* lexeme) {
  return isdigit(lexeme[0]) || l_isfloat_wo_integral_part(lexeme);
}
bool l_issepart(char* lexeme) {
  return lexeme[0] == ',' && strlen(lexeme) == 1;
}
bool l_isfunctn(char* lexeme) {
  int key = keyfromstring(lexeme, FunctionLUT, FunctionLUT_size);
  return key != BAD_KEY;
}
bool l_isoperat(char* lexeme) {
  int key = keyfromstring(lexeme, OperatorLUT, OperatorLUT_size);
  return key != BAD_KEY;
}

TokenType match_token_type(char* lexeme) {
  if (l_isparenl(lexeme)) return PARENTHESIS_LEFT;
  if (l_isparenr(lexeme)) return PARENTHESIS_RIGHT;
  if (l_isnumber(lexeme)) return LITERAL;
  if (l_issepart(lexeme)) return FUNCTION_ARGUMENT_SEPARATOR;
  if (l_isfunctn(lexeme)) return FUNCTION;
  if (l_isoperat(lexeme)) return OPERATOR;
  if (l_isvariab(lexeme)) return VARIABLE;
  return BAD_TOKEN;
}

Token create_token(const char* lexeme, TokenType type) {
  return (Token){type, lexeme};
}
Token read_token(const char* src, size_t* pos) {
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

bool strisempty(const char* str) { return strlen(str) == 0; }

TokenNode* tokenize(const char* src) {
  assert(src != NULL);
  src = remove_spaces(src);
  if (strisempty(src)) {
    return NULL;
  }
  size_t pos = 0;
  Token token = read_token(src, &pos);
  TokenNode* list_head = create_token_node(token);
  TokenNode* token_node = list_head;
  while (pos < strlen(src)) {
    token = read_token(src, &pos);
    token_node->next = create_token_node(token);
    token_node = token_node->next;
  }
  return list_head;
}

void print_tokens(const char* const src, const TokenNode* const list) {
  const TokenNode* node = list;
  printf("Source string: \"%s\"\n", src);
  while (node) {
    Token token = node->token;
    char* type = stringfromkey(token.type, TokenLUT, TokenLUT_size);
    printf("Type: \"%s\", Value: \"%s\"\n", type, token.lexeme);
    node = node->next;
  }
}

int main(void) {
  char expr[] = "1.21. + 2*cos(2x)";

  TokenNode* token_list = tokenize(expr);
  print_tokens(expr, token_list);
}
