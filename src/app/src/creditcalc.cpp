#include "creditcalc.hpp"
#include "./ui_creditcalc.h"

CreditCalc::CreditCalc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CreditCalc)
{
    ui->setupUi(this);
}

CreditCalc::~CreditCalc()
{
    delete ui;
}
