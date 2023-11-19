#ifndef DEPOSITCALC_H
#define DEPOSITCALC_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class DepositCalc;
}
QT_END_NAMESPACE

class DepositCalc : public QMainWindow
{
    Q_OBJECT

public:
    DepositCalc(QWidget *parent = nullptr);
    ~DepositCalc();

private:
    Ui::DepositCalc *ui;
};
#endif // DEPOSITCALC_H
