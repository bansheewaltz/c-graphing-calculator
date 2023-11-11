#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stringutils.h"
#include "token.h"

#define EMPTY (-1)

typedef struct TokenQueue {
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
  queue->capacity = capacity;
  queue->front = 0;
  queue->rear = EMPTY;
  return queue;
}

size_t tkn_queue_getsize(const TokenQueue* const queue) {
  assert(queue != NULL);
  return queue->rear - queue->front + 1;
}

bool tkn_queue_isfull(const TokenQueue* const queue) {
  assert(queue != NULL);
  return queue->rear == queue->capacity - 1;
}

bool tkn_queue_isempty(const TokenQueue* const queue) {
  assert(queue != NULL);
  return queue->rear == EMPTY || queue->rear < queue->front;
}

void tkn_queue_enqueue(TokenQueue* const queue, Token token) {
  assert(queue != NULL);
  assert(!tkn_queue_isfull(queue));
  queue->rear++;
  queue->array[queue->rear] = token;
}

Token tkn_queue_dequeue(TokenQueue* const queue) {
  assert(queue != NULL);
  assert(!tkn_queue_isempty(queue));
  return queue->array[queue->front++];
}

char* tkn_queue_tostr(const TokenQueue* const queue) {
  assert(queue != NULL);
  char* res = NULL;
  for (int i = 0; i <= queue->rear; i++) {
    const char* lexeme = queue->array[i].str;
    string_add(&res, lexeme);
  }
  return res;
}
