#include "error_handling.h"

#include <stdio.h>
#include <stdlib.h>

const char* const ErrorLUT[] = {
    [E_EMPTY_SOURCE_STRING] = "There is no input.",
    [E_BAD_TOKEN] =
        "There is a symbol in the expression string that the calculator does "
        "not work with.",
    [E_PARENTHESES_INVALID_SEQUENCE] =
        "There is a point in the expression string, where the closing "
        "parenthesis does not match any of the opening ones.",
    [E_PARENTHESES_NOT_BALANCED] =
        "The count of opening parentheses does not match with the count of "
        "closing ones.",
    [E_EMPTY_EXPRESSION] =
        "There are no any literals or variables, so there is nothing to "
        "calculate.",
    [E_SEPARATOR_OUT_OF_CONTEXT] =
        "There is an argument separator located not inside a function.",
};

void terminate(ErrorCode error) {
  const char* desc = ErrorLUT[error];
  fprintf(stderr, "%s\n", desc);
  exit(error);
}
