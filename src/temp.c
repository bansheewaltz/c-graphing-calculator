#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  NUMBER = 1,
  OPERATOR = 2,
  FUNCTION = 3,
  PARANTHESIS_LEFT = 4,
  PARANTHESIS_RIGHT = 5
} token_types;

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

typedef char* str;
int tokenize(str const expr, str* tokens, int token_count) {
  for (int i = 0; i < strlen(expr); i++) {
    char ch = expr[i];
    if (is_operator(ch)) {
    }
  }
}
