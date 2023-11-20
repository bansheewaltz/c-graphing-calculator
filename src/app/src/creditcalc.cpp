#include "creditcalc.hpp"

#include "./ui_creditcalc.h"
#include "QtWidgets/qmenubar.h"
#include "smartcalc.h"

CreditCalc::CreditCalc(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);

  QAction *exprAct = new QAction("Expression", this);
  connect(exprAct, &QAction::triggered,
          [this] { emit switchToExpr(), hide(); });
  exprAct->setShortcut(Qt::CTRL | Qt::Key_1);

  QAction *depositAct = new QAction("Deposit", this);
  connect(depositAct, &QAction::triggered,
          [this] { emit switchToDeposit(), hide(); });
  depositAct->setShortcut(Qt::CTRL | Qt::Key_2);

  QAction *creditAct = new QAction("Credit", this);
  QMenu *modeMenu = menuBar()->addMenu("Mode");
  modeMenu->addAction(exprAct);
  modeMenu->addAction(depositAct);
  modeMenu->addAction(creditAct);
}

CreditCalc::~CreditCalc() { delete ui; }

void CreditCalc::on_calculate_payments_clicked() {
  double pay_mnth = 0, total_int_exp = 0, pay_tot = 0;
  double loan_amount = ui->loan_amount_sb->value();
  double interest_rate = ui->interest_rate_sb->value();
  double period = ui->loan_period_sb->value();
  int terms = ui->loan_payment_type_cb->currentIndex();
  ui->payment_schedule_display->setText("");

  enum payment_type { FIXED = 0, DIFFERENTIATED = 1 };
  if (terms == FIXED) {
    SmartCalcError rc =
        smartcalc_credit_fixed_payment(loan_amount, period, interest_rate,
                                       &pay_mnth, &total_int_exp, &pay_tot);
    if (rc == SMARTCALC_SUCCESS) {
      ui->monthly_payment_display->setText(QString::number(pay_mnth, 'f', 2));
      ui->overpayment_display->setText(QString::number(total_int_exp, 'f', 2));
      ui->total_payment_display->setText(QString::number(pay_tot, 'f', 2));
    } else {
      ui->monthly_payment_display->setText("Error");
      ui->overpayment_display->setText("Error");
      ui->total_payment_display->setText("Error");
    }
  }
  if (terms == DIFFERENTIATED) {
    ui->monthly_payment_display->setText("See below");
    QString payments_schedule;
    double fixed_payment, interest_payment;
    for (int i = 1; i <= period; i++) {
      SmartCalcError rc =
          smartcalc_credit_diff_payment(loan_amount, period, i, interest_rate,
                                        &fixed_payment, &interest_payment);
      if (rc == SMARTCALC_SUCCESS) {
        double single_payment = fixed_payment + interest_payment;
        pay_tot += single_payment;
        payments_schedule +=
            QString("month %1 - %2 (fixed = %3, interest = %4)\n")
                .arg(i)
                .arg(QString::number(single_payment, 'f', 2))
                .arg(QString::number(fixed_payment, 'f', 2))
                .arg(QString::number(interest_payment, 'f', 2));
      }
    }

    total_int_exp = pay_tot - loan_amount;
    ui->overpayment_display->setText(QString::number(total_int_exp, 'f', 2));
    ui->total_payment_display->setText(QString::number(pay_tot, 'f', 2));
    ui->payment_schedule_display->setText(payments_schedule);
  }
}
