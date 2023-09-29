#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_

typedef enum ErrorCode {
  E_SUCCESS,
  E_EMPTY_SOURCE_STRING,
  E_BAD_TOKEN,
  E_PARENTHESES_INVALID_SEQUENCE,
  E_PARENTHESES_NOT_BALANCED,
  E_EMPTY_EXPRESSION,
  E_SEPARATOR_OUT_OF_CONTEXT,
} ErrorCode;

extern const char* const ErrorLUT[];

void terminate(ErrorCode error);

#endif  // ERROR_HANDLING_H_
