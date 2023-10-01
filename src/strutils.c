#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool strisempty(const char* const str) { return strlen(str) == 0; }

int strcount(const char* const str, char ch) {
  assert(str != NULL);
  int count = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == ch) count++;
  }
  return count;
}

char* strzcreate(size_t len) {
  char* res = calloc(len + 1, sizeof(char));
  assert(res != NULL);
  return res;
}

char* strremove(const char* src, char ch) {
  assert(src != NULL);
  int ch_count = strcount(src, ch);
  size_t res_len = strlen(src) - ch_count;
  char* res = strzcreate(res_len);
  char* iter = res;
  while (*src != '\0') {
    if (*src != ch) {
      *iter = *src;
      iter++;
    }
    src++;
  }
  *iter = '\0';
  return res;
}

void stradd(char** dest, const char* addition) {
  if (addition == NULL || dest == NULL) return;
  if (*dest == NULL) {
    *dest = strzcreate(strlen(addition));
    sprintf(*dest, "%s", addition);
    return;
  }
  size_t total_len = strlen(*dest) + strlen(addition) + 1;  // +1 for whitespace
  char* res = strzcreate(total_len);
  sprintf(res, "%s %s", *dest, addition);
  free(*dest);
  *dest = res;
}
