#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <stdbool.h>
#include <stddef.h>

bool string_isempty(const char *const str);
int string_count(const char *const str, char ch);
char *string_createz(size_t len);
char *string_remove(const char *src, char ch);
void string_add(char **dest, const char *addition);

#endif  // STRINGUTILS_H_
