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

ErrorCode validate_tokens(const TokenNode* const list) {
  if (list == NULL) {
    return E_EMPTY_SOURCE_STRING;
  }
  int parentheses_balance = 0;
  bool term_presence = false;
  const TokenNode* node = list;
  while (node) {
    Token token = node->token;
    if (token.type == BAD_TOKEN) return E_BAD_TOKEN;
    if (token.type == LITERAL || token.type == VARIABLE) term_presence = true;
    if (token.type == PARENTHESIS_LEFT) parentheses_balance++;
    if (token.type == PARENTHESIS_RIGHT) parentheses_balance--;
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
TokenStack tkn_stack_create(int capacity) {
  TokenStack stack;
  stack.array = malloc(sizeof(Token) * capacity);
  assert(stack.array != NULL);
  stack.top = EMPTY;
  return stack;
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

TokenQueue tkn_queue_create(int capacity) {
  TokenQueue queue;
  queue.array = malloc(sizeof(Token) * capacity);
  assert(queue.array != NULL);
  queue.front = 0;
  queue.rear = EMPTY;
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
const int PrecedenceLUT[] = {
    [ADDITION] = 1, [SUBTRACTION] = 1, [MULTIPLICATION] = 2,
    [DIVISION] = 2, [MODULUS] = 2,     [POWER] = 3,
};

typedef enum {
  ASSOC_LEFT,
  ASSOC_RIGHT,
} Associativity;
const int AssociativityLUT[] = {
    [ADDITION] = ASSOC_LEFT,       [SUBTRACTION] = ASSOC_LEFT,
    [MULTIPLICATION] = ASSOC_LEFT, [DIVISION] = ASSOC_LEFT,
    [MODULUS] = ASSOC_LEFT,        [POWER] = ASSOC_LEFT,

};

void infix_to_postfix(const TokenNode* const list_head) {
  int token_count = tkn_list_get_length(list_head);
  TokenStack op_stack = tkn_stack_create(token_count);
  TokenQueue out_queue = tkn_queue_create(token_count);

  const TokenNode* node = list_head;
  while (node) {
    Token token = node->token;
    switch (token.type) {
      case LITERAL:
      case VARIABLE:
      case FUNCTION_ARGUMENT:
        tkn_queue_enqueue(&out_queue, token);
        break;
      case FUNCTION:
      case PARENTHESIS_LEFT:
        tkn_stack_push(&op_stack, token);
        break;
      case FUNCTION_ARGUMENT_SEPARATOR:
        while (tkn_stack_peek(&op_stack).type != PARENTHESIS_LEFT) {
          Token operator= tkn_stack_pop(&op_stack);
          tkn_queue_enqueue(&out_queue, operator);
        }
        assert(tkn_stack_peek(&op_stack).type == PARENTHESIS_LEFT);
        break;
      case OPERATOR:
        while (tkn_stack_peek(&op_stack).type == OPERATOR) {
          Token operator1 = token;
          int prec1 = PrecedenceLUT[operator1.type];
          int prec2 = PrecedenceLUT[tkn_stack_peek(&op_stack).type];
          Associativity assoc1 = AssociativityLUT[operator1.type];
          if (!(prec2 > prec1 || prec1 == prec2 && assoc1 == ASSOC_LEFT)) {
            break;
          }
          Token operator2 = tkn_stack_pop(&op_stack);
          tkn_queue_enqueue(&out_queue, operator2);
        }
        tkn_stack_push(&op_stack, token);
        break;
      case PARENTHESIS_RIGHT:
        while (tkn_stack_peek(&op_stack).type != PARENTHESIS_LEFT) {
          Token operator= tkn_stack_pop(&op_stack);
          tkn_stack_push(&op_stack, operator);
        }
        assert(tkn_stack_peek(&op_stack).type == PARENTHESIS_LEFT);
        (void)tkn_stack_pop(&op_stack);
        if (tkn_stack_peek(&op_stack).type == FUNCTION) {
          Token function = tkn_stack_pop(&op_stack);
          tkn_queue_enqueue(&out_queue, function);
        }
        break;
      default:
        assert(!token.type);
    }
    node = node->next;
  }
  while (!tkn_stack_isempty(&op_stack)) {
    assert(tkn_stack_peek(&op_stack).type != PARENTHESIS_LEFT);
    Token operator= tkn_stack_pop(&op_stack);
    tkn_queue_enqueue(&out_queue, operator);
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
