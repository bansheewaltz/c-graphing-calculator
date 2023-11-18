#ifndef MainWindow_HPP_
#define MainWindow_HPP_

#include <QAbstractButton>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 private:
  double x;
  double xmin, xmax, ymin, ymax;
  QVector<double> x_graph, y_graph;
  QString error_message = "Error: Invalid input expression";
  enum UiSymbols { PLUS, MINUS, MUL, DIV, MOD, POW, XVAR, ENUM_SIZE };
  const int operators_count = 6;
  std::vector<QChar> ui_symbols_lut;

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  bool verifySequenceCorrectness(QString str, QChar sym);
  bool isOperator(QChar sym);
  bool isGraphOpen();
  void updateGraph();
  void prepareDisplay(QAbstractButton *button);

 private slots:
  void on_eq_clicked();
  void on_ac_clicked();
  void on_del_clicked();
  void on_xVal_textChanged(const QString &arg1);
  void on_symbolButtonGroup_buttonClicked(QAbstractButton *button);
  void on_functionButtonGroup_buttonClicked(QAbstractButton *button);
  void on_plot_clicked();

 private:
  Ui::MainWindow *ui;
  QPropertyAnimation *window_animation = nullptr;
  void animateWindowSize();
};

#endif  // MainWindow_HPP_
