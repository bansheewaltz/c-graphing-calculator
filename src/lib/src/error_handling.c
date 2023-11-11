#include <stdio.h>
#include <stdlib.h>

#include "smartcalc.h"

static const char* const ErrorLUT[] = {
    [SMARTCALC_EMPTY_SOURCE_STRING] = "There is no input.",
    [SMARTCALC_BAD_TOKEN] =
        "There is a symbol in the expression string that the calculator does "
        "not work with.",
    [SMARTCALC_PARENTHESES_INVALID_SEQUENCE] =
        "There is a point in the expression string, where the closing "
        "parenthesis does not match any of the opening ones.",
    [SMARTCALC_PARENTHESES_NOT_BALANCED] =
        "The count of opening parentheses does not match with the count of "
        "closing ones.",
    [SMARTCALC_EMPTY_EXPRESSION] =
        "There are no any literals or variables, so there is nothing to "
        "calculate.",
    [SMARTCALC_SEPARATOR_OUT_OF_CONTEXT] =
        "There is an argument separator located not inside a function.",
};

void smartcalc_error_terminate(SmartCalcError error) {
  const char* desc = ErrorLUT[error];
  fprintf(stderr, "%s\n", desc);
  exit(error);
}

const char* smartcalc_error_get_description(SmartCalcError error) {
  return ErrorLUT[error];
}
