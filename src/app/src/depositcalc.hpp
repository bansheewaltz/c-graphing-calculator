#ifndef DEPOSITCALC_H
#define DEPOSITCALC_H

#include <QMainWindow>

#include "smartcalc.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DepositCalc;
}
QT_END_NAMESPACE

class DepositCalc : public QMainWindow
{
    Q_OBJECT

    DepositStack dstack;

   public:
    DepositCalc(QWidget *parent = nullptr);
    ~DepositCalc();

private:
    Ui::DepositCalc *ui;
   private slots:
    void on_button_add_wthwal_clicked();
    void on_button_revert_op_clicked();
    void on_button_add_repl_clicked();
    void on_button_Calculate_Deposit_clicked();

    // multiwindow functionality
   private:
    void moveEvent(QMoveEvent *event) { emit newPosSet(this->pos()); }
   signals:
    void switchToExpr();
    void switchToCredit();
    void newPosSet(QPoint);
   public slots:
    void setActive() {
      show();
      activateWindow();
      raise();
    }
    void setNewPos(QPoint newpos) { this->move(newpos.x(), newpos.y()); }
};
#endif // DEPOSITCALC_H
