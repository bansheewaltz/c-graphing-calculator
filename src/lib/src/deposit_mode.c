#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

#include "smartcalc.h"

void smartcalc_deposit_stackinit(DepositStack *stack) { stack->top = -1; }
void smartcalc_deposit_stackpush(DepositStack *stack,
                                 DepositOperation operation) {
  assert(stack->top != STACK_SIZE - 1);
  stack->operations[++stack->top] = operation;
}
DepositOperation smartcalc_deposit_stackpop(DepositStack *stack) {
  assert(stack->top != -1);
  return stack->operations[stack->top--];
}
bool smartcalc_deposit_stackisempty(DepositStack *stack) {
  return stack->top == -1;
}
bool smartcalc_deposit_stackisfull(DepositStack *stack) {
  return stack->top == STACK_SIZE - 1;
}

SmartCalcError smartcalc_deposit_calculate(
    DepositStack dstack, double depo_amount, double period, double i_rate,
    double t_rate, int terms, int cap, double *total_interest,
    double *total_tax, double *end_amount) {
  if (depo_amount <= 0 || period <= 0) {
    return !SMARTCALC_ERR_SUCCESS;
  }

  *end_amount = depo_amount;
  for (int i = 1; i <= period; i++) {
    double current_int = 0;
    double current_tax = 0;
    for (int j = 0; j <= dstack.top; j++) {
      if (dstack.operations[j].month == i) {
        *end_amount += dstack.operations[j].amount;
      }
    }
    if (terms == 0) {
      current_int = (*end_amount * i_rate / 12 / 100) +
                    (*total_interest * i_rate / 12 / 100 * cap);
    } else if (terms == 1 && (i % 3 == 0)) {
      current_int = (*end_amount * i_rate / 4 / 100) +
                    (*total_interest * i_rate / 4 / 100 * cap);
    } else if (terms == 2 && (i % 12) == 0) {
      current_int =
          (*end_amount * i_rate / 100) + (*total_interest * i_rate / 100 * cap);
    }
    current_tax = current_int * t_rate / 100;
    *total_interest += current_int;
    *total_tax += current_tax;
  }
  *end_amount += (*total_interest - *total_tax);
  //    if ((*end_amount < 0) || (*total_interest < 0) || (*total_tax < 0)) {
  //      error_code = EXIT_ERROR;
  //      *end_amount = 0;
  //      *total_interest = 0;
  //      *total_tax = 0;
  //    } else {
  *end_amount = round(*end_amount * 100) / 100;
  *total_interest = round(*total_interest * 100) / 100;
  *total_tax = round(*total_tax * 100) / 100;
  //    }

  return SMARTCALC_ERR_SUCCESS;
}
