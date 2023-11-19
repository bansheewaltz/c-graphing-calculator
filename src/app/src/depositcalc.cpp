#include "depositcalc.hpp"
#include "./ui_depositcalc.h"

DepositCalc::DepositCalc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DepositCalc)
{
    ui->setupUi(this);
}

DepositCalc::~DepositCalc()
{
    delete ui;
}
