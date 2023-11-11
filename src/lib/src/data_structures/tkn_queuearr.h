#ifndef TKN_QUEUEARR_H_
#define TKN_QUEUEARR_H_

#include <stdbool.h>

#include "token.h"

typedef struct TokenQueue TokenQueue;

TokenQueue *tkn_queue_create(int capacity);
bool tkn_queue_isfull(const TokenQueue *const queue);
bool tkn_queue_isempty(const TokenQueue *const queue);
void tkn_queue_enqueue(TokenQueue *const queue, Token token);
Token tkn_queue_dequeue(TokenQueue *const queue);
size_t tkn_queue_getsize(const TokenQueue *const queue);
char *tkn_queue_tostr(const TokenQueue *const queue);

#endif  // TKN_QUEUEARR_H_
