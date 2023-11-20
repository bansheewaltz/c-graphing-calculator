#ifndef CREDITCALC_HPP
#define CREDITCALC_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class CreditCalc;
}
QT_END_NAMESPACE

class CreditCalc : public QMainWindow
{
    Q_OBJECT

public:
    CreditCalc(QWidget *parent = nullptr);
    ~CreditCalc();

private:
 Ui::CreditCalc *ui;
private slots:
 void on_calculate_payments_clicked();

 // multiwindow functionality
private:
 void moveEvent(QMoveEvent *event) { emit newPosSet(this->pos()); }
signals:
 void switchToExpr();
 void switchToDeposit();
 void newPosSet(QPoint);
public slots:
 void setActive() {
   show();
   activateWindow();
   raise();
 }
 void setNewPos(QPoint newpos) { this->move(newpos.x(), newpos.y()); }
};
#endif // CREDITCALC_HPP
