#ifndef STRUTILS_H_
#define STRUTILS_H_

#include <stdbool.h>
#include <stddef.h>

bool iscomma(char ch);
bool isparen(char ch);
bool isradixp(char ch);
bool isdigitf(char ch);
bool strisempty(const char *const str);
int strcount(const char *const str, char ch);
char *strzcreate(size_t len);
char *strremove(const char *src, char ch);
void stradd(char **dest, const char *addition);

#endif  // STRUTILS_H_
