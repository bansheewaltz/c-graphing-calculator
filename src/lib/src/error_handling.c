#include <stdio.h>
#include <stdlib.h>

#include "smartcalc.h"

static const char* const ErrorLUT[] = {
    [SMARTCALC_ERR_EMPTY_SOURCE_STRING] = "There is no input.",
    [SMARTCALC_ERR_BAD_TOKEN] =
        "There is a symbol in the expression string that the calculator does "
        "not work with.",
    [SMARTCALC_ERR_INVALID_PARENTHESES_SEQUENCE] =
        "Not every opening bracket has a corresponding close one.",
    [SMARTCALC_ERR_EMPTY_EXPRESSION] =
        "There are no any literals or variables, so there is nothing to "
        "calculate.",
    [SMARTCALC_ERR_SEPARATOR_OUT_OF_CONTEXT] =
        "There is an argument separator located not inside a function.",
};

const char* smartcalc_error_get_description(SmartCalcError error) {
  return ErrorLUT[error];
}
