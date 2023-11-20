#include "depositcalc.hpp"

#include "./ui_depositcalc.h"
#include "QtWidgets/qmenubar.h"
#include "smartcalc.h"

DepositCalc::DepositCalc(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositCalc) {
  ui->setupUi(this);

  QAction *exprAct = new QAction("Expression", this);
  connect(exprAct, &QAction::triggered,
          [this] { emit switchToExpr(), hide(); });
  exprAct->setShortcut(Qt::CTRL | Qt::Key_1);

  QAction *depositAct = new QAction("Deposit", this);

  QAction *creditAct = new QAction("Credit", this);
  connect(creditAct, &QAction::triggered,
          [this] { emit switchToCredit(), hide(); });
  creditAct->setShortcut(Qt::CTRL | Qt::Key_3);

  QMenu *modeMenu = menuBar()->addMenu("Mode");
  modeMenu->addAction(exprAct);
  modeMenu->addAction(depositAct);
  modeMenu->addAction(creditAct);

  smartcalc_deposit_stackinit(&dstack);
}

DepositCalc::~DepositCalc() { delete ui; }

void DepositCalc::on_button_add_repl_clicked() {
  DepositOperation operation;
  operation.amount = ui->box_oper_amount->value();
  operation.month = ui->box_oper_month->value();
  smartcalc_deposit_stackpush(&dstack, operation);
  QString planned_operations;
  for (int i = 0; i <= dstack.top; i++) {
    planned_operations +=
        QString("month %1: %2\n")
            .arg(dstack.operations[i].month)
            .arg(QString::number(dstack.operations[i].amount, 'f', 2));
  }
  ui->display_deposit_ops->setText(planned_operations);
}

void DepositCalc::on_button_add_wthwal_clicked() {
  DepositOperation operation;
  operation.amount = -(ui->box_oper_amount->value());
  operation.month = ui->box_oper_month->value();
  smartcalc_deposit_stackpush(&dstack, operation);
  QString planned_operations;
  for (int i = 0; i <= dstack.top; i++) {
    planned_operations +=
        QString("month %1: %2\n")
            .arg(dstack.operations[i].month)
            .arg(QString::number(dstack.operations[i].amount, 'f', 2));
  }
  ui->display_deposit_ops->setText(planned_operations);
}

void DepositCalc::on_button_revert_op_clicked() {
  QString planned_operations;
  if (!smartcalc_deposit_stackisempty(&dstack)) {
    smartcalc_deposit_stackpop(&dstack);
    for (int i = 0; i <= dstack.top; i++) {
      planned_operations +=
          QString("month %1: %2\n")
              .arg(dstack.operations[i].month)
              .arg(QString::number(dstack.operations[i].amount, 'f', 2));
    }
    ui->display_deposit_ops->setText(planned_operations);
  }
}

void DepositCalc::on_button_Calculate_Deposit_clicked() {
  double total_interest = 0, total_tax = 0, end_amount = 0;
  double depo_amount = ui->box_dep_amount->value();
  double period = ui->box_dep_period->value();
  double i_rate = ui->box_i_rate->value();
  double t_rate = ui->box_t_rate->value();
  int terms = ui->CP_Periodicity->currentIndex();
  int cap = ui->box_capitalzation->isChecked() ? 1 : 0;

  SmartCalcError rc = smartcalc_deposit_calculate(
      dstack, depo_amount, period, i_rate, t_rate, terms, cap, &total_interest,
      &total_tax, &end_amount);

  if (rc == SMARTCALC_SUCCESS) {
    ui->display_total_int->setText(QString::number(total_interest, 'f', 2));
    ui->display_total_tax->setText(QString::number(total_tax, 'f', 2));
    ui->display_end_amount->setText(QString::number(end_amount, 'f', 2));
  } else {
    ui->display_total_int->setText("Error");
    ui->display_total_tax->setText("Error");
    ui->display_end_amount->setText("Error");
  }
}
