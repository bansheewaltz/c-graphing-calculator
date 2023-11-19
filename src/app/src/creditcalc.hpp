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
};
#endif // CREDITCALC_HPP
