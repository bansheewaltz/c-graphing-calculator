#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BAD_KEY -1
typedef struct key_value_pair {
  char* value;
  int key;
} KeyValuePair;
typedef enum error_code {
  E_SUCCESS,
  E_EMPTY_SOURCE_STRING,
  E_BAD_TOKEN,
  E_PARENTHESES_INVALID_SEQUENCE,
  E_PARENTHESES_NOT_BALANCED,
  E_EMPTY_EXPRESSION,
  E_SEPARATOR_OUT_OF_CONTEXT,
} ErrorCode;
const KeyValuePair ErrorLUT[] = {
    {"There is no input.", E_EMPTY_SOURCE_STRING},
    {"There is a symbol in the expression string that the calculator does not "
     "work with.",
     E_BAD_TOKEN},
    {"There is a point in the expression string, where the closing parenthesis "
     "does not match any of the opening ones.",
     E_PARENTHESES_INVALID_SEQUENCE},
    {"The count of opening parentheses does not match with the count of "
     "closing ones.",
     E_PARENTHESES_NOT_BALANCED},
    {"There are no any literals or variables, so there is nothing to "
     "calculate.",
     E_EMPTY_EXPRESSION},
    {"There is an argument separator located not inside a function.",
     E_SEPARATOR_OUT_OF_CONTEXT},
};
const int ErrorLUT_size = sizeof(ErrorLUT) / sizeof(ErrorLUT[0]);

int keyfromstring(const char* value, const KeyValuePair lut[], int lut_size) {
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
  TT_YET_UNDEFINED,
  TT_LITERAL,
  TT_VARIABLE,
  TT_OPERATOR,
  TT_UNARY_OPERATOR,
  TT_FUNCTION,
  TT_FUNCTION_ARGUMENT,
  TT_FUNCTION_ARGUMENT_SEPARATOR,
  TT_PARENTHESIS_LEFT,
  TT_PARENTHESIS_RIGHT,
  TT_BAD_TOKEN
} TokenType;
const KeyValuePair TokenLUT[] = {
    {"literal", TT_LITERAL},
    {"variable", TT_VARIABLE},
    {"operator", TT_OPERATOR},
    {"unary operator", TT_UNARY_OPERATOR},
    {"function", TT_FUNCTION},
    {"function argument", TT_FUNCTION_ARGUMENT},
    {"function argument separator", TT_FUNCTION_ARGUMENT_SEPARATOR},
    {"parenthesis left", TT_PARENTHESIS_LEFT},
    {"parenthesis right", TT_PARENTHESIS_RIGHT},
    {"bad token", TT_BAD_TOKEN},
};
const int TokenLUT_size = sizeof(TokenLUT) / sizeof(TokenLUT[0]);

typedef enum operator{
  OP_ADDITION,
  OP_SUBTRACTION,
  OP_MULTIPLICATION,
  OP_DIVISION,
  OP_POWER,
  OP_MODULUS,
} Operator;
const KeyValuePair OperatorLUT[] = {
    {"+", OP_ADDITION}, {"-", OP_SUBTRACTION}, {"*", OP_MULTIPLICATION},
    {"/", OP_DIVISION}, {"^", OP_POWER},       {"mod", OP_MODULUS},
};
const int OpLUT_size = sizeof(OperatorLUT) / sizeof(OperatorLUT[0]);
typedef enum function {
  FN_ARC_COSINE,
  FN_ARC_COTANGENT,
  FN_ARC_SINE,
  FN_ARC_TANENGT,
  FN_COSINE,
  FN_COTANGENT,
  FN_LOGARITHM_COMMON,
  FN_LOGARITHM_NATURAL,
  FN_SINE,
  FN_SQUARE_ROOT,
  FN_TANGENT,
} Function;
const KeyValuePair FunctionLUT[] = {
    {"sin", FN_SINE},
    {"cos", FN_COSINE},
    {"tan", FN_TANGENT},
    {"cot", FN_COTANGENT},
    {"asin", FN_ARC_SINE},
    {"acos", FN_ARC_COSINE},
    {"atan", FN_ARC_TANENGT},
    {"acot", FN_ARC_COTANGENT},
    {"sqrt", FN_SQUARE_ROOT},
    {"ln", FN_LOGARITHM_NATURAL},
    {"log", FN_LOGARITHM_COMMON},
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

bool lxm_isvariab(char* lexeme) { return isalpha(lexeme[0]); }
bool lxm_isparenl(char* lexeme) { return lexeme[0] == '('; }
bool lxm_isparenr(char* lexeme) { return lexeme[0] == ')'; }

bool lxm_isfloat_wo_integralxm_part(char* lexeme) {
  return isradixp(lexeme[0]) && strlen(lexeme) > 1;
}
bool lxm_isnumber(char* lexeme) {
  return isdigit(lexeme[0]) || lxm_isfloat_wo_integralxm_part(lexeme);
}
bool lxm_issepart(char* lexeme) {
  return lexeme[0] == ',' && strlen(lexeme) == 1;
}
bool lxm_isfunctn(char* lexeme) {
  int key = keyfromstring(lexeme, FunctionLUT, FunctionLUT_size);
  return key != BAD_KEY;
}
bool lxm_isoperat(char* lexeme) {
  int key = keyfromstring(lexeme, OperatorLUT, OpLUT_size);
  return key != BAD_KEY;
}

TokenType match_token_type(char* lexeme) {
  if (lxm_isparenl(lexeme)) return TT_PARENTHESIS_LEFT;
  if (lxm_isparenr(lexeme)) return TT_PARENTHESIS_RIGHT;
  if (lxm_isnumber(lexeme)) return TT_LITERAL;
  if (lxm_issepart(lexeme)) return TT_FUNCTION_ARGUMENT_SEPARATOR;
  if (lxm_isfunctn(lexeme)) return TT_FUNCTION;
  if (lxm_isoperat(lexeme)) return TT_OPERATOR;
  if (lxm_isvariab(lexeme)) return TT_VARIABLE;
  return TT_BAD_TOKEN;
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

ErrorCode validate_tokens(const TokenNode* const list) {
  if (list == NULL) {
    return E_EMPTY_SOURCE_STRING;
  }
  int parentheses_balance = 0;
  bool term_presence = false;
  const TokenNode* node = list;
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

void terminate(ErrorCode error) {
  char* desc = stringfromkey(error, ErrorLUT, ErrorLUT_size);
  fprintf(stderr, "%s\n", desc);
  exit(error);
}

// void cleanup_tokens(const TokenNode* const list) {
//   const TokenNode* node = list;
//   TokenType last_type;
//   bool last_minus;
//   while (node) {
//     Token token = node->token;

//     node = node->next;
//   }
// }

int tkn_list_get_length(const TokenNode* const list_head) {
  const TokenNode* node = list_head;
  int count = 0;
  while (node) {
    count++;
    node = node->next;
  }
  return count;
}

/* REVERSE POLISH NOTATION */

typedef struct token_stack {
  int capacity;
  Token* array;
  int top;
} TokenStack;
#define EMPTY -1
TokenStack* tkn_stack_create(int capacity) {
  TokenStack* stack = malloc(sizeof(TokenStack) * capacity);
  assert(stack != NULL);
  stack->array = malloc(sizeof(Token) * capacity);
  assert(stack->array != NULL);
  stack->top = EMPTY;
  return stack;
}
void tkn_stack_remove(TokenStack* stack) {
  if (stack == NULL) return;
  if (stack->array) {
    free(stack->array);
  }
  free(stack);
}
bool tkn_stack_isfull(const TokenStack* const stack) {
  assert(stack != NULL);
  return stack->top == stack->capacity - 1;
}
bool tkn_stack_isempty(const TokenStack* const stack) {
  assert(stack != NULL);
  return stack->top == EMPTY;
}
void tkn_stack_push(TokenStack* const stack, Token token) {
  assert(stack != NULL);
  assert(!tkn_stack_isfull(stack));
  stack->top++;
  stack->array[stack->top] = token;
}
Token tkn_stack_pop(TokenStack* const stack) {
  assert(stack != NULL);
  assert(!tkn_stack_isempty(stack));
  return stack->array[stack->top--];
}
Token tkn_stack_peek(TokenStack* const stack) {
  assert(stack != NULL);
  assert(!tkn_stack_isempty(stack));
  return stack->array[stack->top];
}

typedef struct token_queue {
  int capacity;
  Token* array;
  int front;
  int rear;
} TokenQueue;

TokenQueue* tkn_queue_create(int capacity) {
  TokenQueue* queue = malloc(sizeof(TokenQueue) * capacity);
  assert(queue != NULL);
  queue->array = malloc(sizeof(Token) * capacity);
  assert(queue->array != NULL);
  queue->front = 0;
  queue->rear = EMPTY;
  return queue;
}
bool tkn_queue_isfull(const TokenQueue* const queue) {
  assert(queue != NULL);
  return queue->rear == queue->capacity - 1;
}
bool tkn_queue_isempty(const TokenQueue* const queue) {
  assert(queue != NULL);
  return queue->rear == EMPTY;
}
void tkn_queue_enqueue(TokenQueue* const queue, Token token) {
  assert(queue != NULL);
  assert(!tkn_queue_isfull(queue));
  queue->front++;
  queue->array[queue->front] = token;
}
Token tkn_queue_dequeue(TokenQueue* const queue) {
  assert(queue != NULL);
  assert(!tkn_queue_isempty(queue));
  return queue->array[queue->front++];
}

/**
 * @brief greater int = greater precedence
 *
 */
typedef int Precedence;
const int PrecedenceLUT[] = {
    [OP_ADDITION] = 1, [OP_SUBTRACTION] = 1, [OP_MULTIPLICATION] = 2,
    [OP_DIVISION] = 2, [OP_MODULUS] = 2,     [OP_POWER] = 3,
};

typedef enum {
  ASSOC_LEFT,
  ASSOC_RIGHT,
} Associativity;
const int AssociativityLUT[] = {
    [OP_ADDITION] = ASSOC_LEFT,       [OP_SUBTRACTION] = ASSOC_LEFT,
    [OP_MULTIPLICATION] = ASSOC_LEFT, [OP_DIVISION] = ASSOC_LEFT,
    [OP_MODULUS] = ASSOC_LEFT,        [OP_POWER] = ASSOC_LEFT,

};

/**
 * @brief Dijkstra's shunting yard algorithm
 *
 * @param list_head
 */
void infix_to_postfix(const TokenNode* const list_head) {
  int token_count = tkn_list_get_length(list_head);
  TokenStack* op_stack = tkn_stack_create(token_count);
  TokenQueue* out_queue = tkn_queue_create(token_count);

  const TokenNode* node = list_head;
  while (node) {
    Token token = node->token;
    switch (token.type) {
      case TT_LITERAL:
      case TT_VARIABLE:
      case TT_FUNCTION_ARGUMENT:
        tkn_queue_enqueue(out_queue, token);
        break;
      case TT_FUNCTION:
      case TT_PARENTHESIS_LEFT:
        tkn_stack_push(op_stack, token);
        break;
      case TT_FUNCTION_ARGUMENT_SEPARATOR:
        while (tkn_stack_peek(op_stack).type != TT_PARENTHESIS_LEFT) {
          Token operator= tkn_stack_pop(op_stack);
          tkn_queue_enqueue(out_queue, operator);
        }
        assert(tkn_stack_peek(op_stack).type == TT_PARENTHESIS_LEFT);
        break;
      case TT_OPERATOR:
        while (tkn_stack_peek(op_stack).type == TT_OPERATOR) {
          Token t1 = token;
          Token t2 = tkn_stack_peek(op_stack);
          Operator op1 = keyfromstring(t1.lexeme, OperatorLUT, OpLUT_size);
          Operator op2 = keyfromstring(t2.lexeme, OperatorLUT, OpLUT_size);
          int prec1 = PrecedenceLUT[op1];
          int prec2 = PrecedenceLUT[op2];
          int assoc1 = AssociativityLUT[op1];
          if (prec2 > prec1 || prec1 == prec2 && assoc1 == ASSOC_LEFT) {
            Token operator2 = tkn_stack_pop(op_stack);
            tkn_queue_enqueue(out_queue, operator2);
          } else {
            break;
          }
        }
        tkn_stack_push(op_stack, token);
        break;
      case TT_PARENTHESIS_RIGHT:
        while (tkn_stack_peek(op_stack).type != TT_PARENTHESIS_LEFT) {
          Token operator= tkn_stack_pop(op_stack);
          tkn_stack_push(op_stack, operator);
        }
        assert(tkn_stack_peek(op_stack).type == TT_PARENTHESIS_LEFT);
        (void)tkn_stack_pop(op_stack);
        if (tkn_stack_peek(op_stack).type == TT_FUNCTION) {
          Token function = tkn_stack_pop(op_stack);
          tkn_queue_enqueue(out_queue, function);
        }
        break;
      default:
        assert(!token.type);
    }
    node = node->next;
  }
  while (!tkn_stack_isempty(op_stack)) {
    assert(tkn_stack_peek(op_stack).type != TT_PARENTHESIS_LEFT);
    Token operator= tkn_stack_pop(op_stack);
    tkn_queue_enqueue(out_queue, operator);
  }
}

int main(void) {
  char expr[] = "1.21 + 2*cos2x, 3x";

  TokenNode* token_list = tokenize(expr);
  ErrorCode error = validate_tokens(token_list);
  if (error) {
    terminate(error);
  }
  // format_token_sequence(token_list);
  print_tokens(expr, token_list);
}
