#include <QApplication>

#include "creditcalc.hpp"
#include "depositcalc.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  //  MainWindow w;
  //  w.show();

  MainWindow mw;
  CreditCalc cc;
  DepositCalc dc;

  QObject::connect(&mw, &MainWindow::switchToDeposit, &dc,
                   &DepositCalc::setActive);
  QObject::connect(&mw, &MainWindow::switchToCredit, &cc,
                   &CreditCalc::setActive);
  QObject::connect(&mw, &MainWindow::newPosSet, &cc, &CreditCalc::setNewPos);
  QObject::connect(&mw, &MainWindow::newPosSet, &dc, &DepositCalc::setNewPos);

  QObject::connect(&dc, &DepositCalc::switchToExpr, &mw,
                   &MainWindow::setActive);
  QObject::connect(&dc, &DepositCalc::switchToCredit, &cc,
                   &CreditCalc::setActive);
  QObject::connect(&dc, &DepositCalc::newPosSet, &mw, &MainWindow::setNewPos);
  QObject::connect(&dc, &DepositCalc::newPosSet, &cc, &CreditCalc::setNewPos);

  QObject::connect(&cc, &CreditCalc::switchToExpr, &mw, &MainWindow::setActive);
  QObject::connect(&cc, &CreditCalc::switchToDeposit, &dc,
                   &DepositCalc::setActive);
  QObject::connect(&cc, &CreditCalc::newPosSet, &mw, &MainWindow::setNewPos);
  QObject::connect(&cc, &CreditCalc::newPosSet, &dc, &DepositCalc::setNewPos);

  mw.show();

  return a.exec();
}
