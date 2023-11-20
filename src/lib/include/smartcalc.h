#ifndef SMARTCALC_H_
#define SMARTCALC_H_

/** @file smartcalc.h */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define SMARTCALC_INPUT_MAX_LEN 256
#define SMARTCALC_SUCCESS 0

typedef enum ErrorCode {
  SMARTCALC_ERR_SUCCESS,
  SMARTCALC_ERR_EMPTY_SOURCE_STRING,
  SMARTCALC_ERR_BAD_TOKEN,
  SMARTCALC_ERR_INVALID_PARENTHESES_SEQUENCE,
  SMARTCALC_ERR_EMPTY_EXPRESSION,
  SMARTCALC_ERR_SEPARATOR_OUT_OF_CONTEXT,
} SmartCalcError;

/**
 * The function smartcalc_expr_analyze analyzes an expression by tokenizing it
 * and printing the source string and the token list.
 *
 * @param expr The `expr` parameter is a pointer to a constant character array,
 * which represents the expression that needs to be analyzed.
 */
void smartcalc_expr_analyze(const char* expr);

/**
 * The function converts an infix expression to postfix notation and returns the
 * result as a string.
 *
 * @param expr The `expr` parameter is a pointer to a character array that
 * represents an infix mathematical expression.
 * @param result The "result" parameter is a pointer to a pointer to a character
 * array. It is used to store the resulting postfix expression as a string.
 *
 * @return a SmartCalcError value.
 */
SmartCalcError smartcalc_expr_infix_to_postfix(const char* expr, char** result);

/**
 * The function evaluates an infix mathematical expression with a variable value
 * and returns the result.
 *
 * @param expr A string representing a mathematical expression in infix
 * notation.
 * @param x The parameter `x` is a double value that represents the value of the
 * variable `x` in the expression. It is used to substitute the variable `x` in
 * the expression before evaluating it.
 * @param result A pointer to a double variable where the result of the
 * expression evaluation will be stored.
 *
 * @return a value of type SmartCalcError.
 */
SmartCalcError smartcalc_expr_infix_evaluate(const char* expr, double x,
                                             double* result);

/**
 * The function returns the description of a given error code.
 *
 * @param error The parameter "error" is of type SmartCalcError, which is an
 * enumeration representing different error codes in the smart calculator
 * program.
 *
 * @return a pointer to a character array (const char*) which represents the
 * description of the given SmartCalcError.
 */
const char* smartcalc_error_get_description(SmartCalcError error);

typedef struct {
  double amount;
  double month;
} DepositOperation;

#define STACK_SIZE 256

typedef struct {
  DepositOperation operations[STACK_SIZE];
  int top;
} DepositStack;

void smartcalc_deposit_stackinit(DepositStack* stack);
void smartcalc_deposit_stackpush(DepositStack* stack,
                                 DepositOperation operation);
DepositOperation smartcalc_deposit_stackpop(DepositStack* stack);
bool smartcalc_deposit_stackisfull(DepositStack* stack);
bool smartcalc_deposit_stackisempty(DepositStack* stack);

/**
 * The function calculates the total interest, total tax, and end amount for a
 * deposit based on various parameters.
 *
 * @param dstack The parameter `dstack` is of type `DepositStack` and represents
 * a stack of deposit operations. It is used to keep track of the deposits made
 * at different months during the period.
 * @param depo_amount The deposit amount (initial amount of money deposited)
 * @param period The "period" parameter represents the duration of the deposit
 * in months.
 * @param i_rate The parameter `i_rate` represents the annual interest rate for
 * the deposit.
 * @param t_rate The parameter `t_rate` represents the tax rate, which is the
 * percentage of the interest that is deducted as tax.
 * @param terms The "terms" parameter in the smartcalc_deposit_calculate
 * function represents the type of deposit term. It can have three possible
 * values:
 * @param cap The parameter "cap" in the smartcalc_deposit_calculate function
 * represents the capitalization frequency of the deposit. It determines how
 * often the interest is added to the deposit amount.
 * @param total_interest A pointer to a double variable that will store the
 * total interest earned over the deposit period.
 * @param total_tax A pointer to a double variable that will store the total tax
 * amount calculated by the function.
 * @param end_amount A pointer to a double variable that will store the final
 * amount after the calculation.
 *
 * @return a value of type SmartCalcError.
 */
SmartCalcError smartcalc_deposit_calculate(
    DepositStack dstack, double depo_amount, double period, double i_rate,
    double t_rate, int terms, int cap, double* total_interest,
    double* total_tax, double* end_amount);

/**
 * The function calculates the fixed monthly payment, total interest expense,
 * and total payment for a loan based on the loan amount, period, and interest
 * rate.
 *
 * @param loan_amount The loan amount is the total amount of money borrowed.
 * @param period The "period" parameter represents the number of months over
 * which the loan will be repaid.
 * @param interest_rate The interest rate is the annual interest rate for the
 * loan.
 * @param pay_mnth The monthly payment amount for the fixed credit.
 * @param int_exp The parameter `int_exp` represents the total interest expense
 * over the loan period.
 * @param pay_tot The parameter `pay_tot` represents the total amount to be paid
 * over the loan period, including both the principal amount and the interest.
 *
 * @return a value of type SmartCalcError.
 */
SmartCalcError smartcalc_credit_fixed_payment(double loan_amount, double period,
                                              double interest_rate,
                                              double* pay_mnth, double* int_exp,
                                              double* pay_tot);

/**
 * The function calculates the fixed payment and interest payment for a credit
 * with a given loan amount, period, interest rate, and initial payment.
 *
 * @param loan_amount The loan amount is the total amount of money borrowed.
 * @param period The period parameter represents the number of months over which
 * the loan will be repaid.
 * @param i The parameter "i" in the function smartcalc_credit_diff_payment
 * represents the current period of the loan. It is used to calculate the
 * current debt by subtracting the fixed payment multiplied by (i - 1) from the
 * loan amount.
 * @param interest_rate The `interest_rate` parameter represents the annual
 * interest rate for the loan.
 * @param fixed_payment The `fixed_payment` parameter is a pointer to a double
 * variable that will store the calculated fixed payment amount.
 * @param interest_payment The parameter `interest_payment` is a pointer to a
 * double variable. It is used to store the calculated interest payment for the
 * given loan amount, period, interest rate, and fixed payment.
 *
 * @return a value of type SmartCalcError.
 */
SmartCalcError smartcalc_credit_diff_payment(double loan_amount, double period,
                                             double i, double interest_rate,
                                             double* fixed_payment,
                                             double* interest_payment);
#ifdef __cplusplus
}
#endif

#endif  // SMARTCALC_H_
