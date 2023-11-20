#include <math.h>

#include "smartcalc.h"

SmartCalcError smartcalc_credit_fixed_payment(double loan_amount, double period,
                                              double interest_rate,
                                              double* pay_mnth, double* int_exp,
                                              double* pay_tot) {
  if (loan_amount <= 0 || period <= 0) {
    return !SMARTCALC_SUCCESS;
  }

  double month_r = (interest_rate / 100) / 12;
  *pay_mnth =
      loan_amount * (month_r + (month_r / (pow((1 + month_r), period) - 1)));
  *int_exp = (*pay_mnth) * period - loan_amount;
  *pay_tot = *pay_mnth * period;
  *pay_mnth = round(*pay_mnth * 100) / 100;
  *int_exp = round(*int_exp * 100) / 100;
  *pay_tot = round(*pay_tot * 100) / 100;

  return SMARTCALC_SUCCESS;
}

SmartCalcError smartcalc_credit_diff_payment(double loan_amount, double period,
                                             double i, double interest_rate,
                                             double* fixed_payment,
                                             double* interest_payment) {
  if (loan_amount <= 0 || period <= 0) {
    return !SMARTCALC_SUCCESS;
  }

    *fixed_payment = loan_amount / period;
    double current_debt = loan_amount - ((*fixed_payment) * (i - 1));
    *interest_payment = current_debt * (interest_rate / 100) / 12;
    *fixed_payment = round(*fixed_payment * 100) / 100;
    *interest_payment = round(*interest_payment * 100) / 100;

    return SMARTCALC_SUCCESS;
}
