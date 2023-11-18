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
  double xmin, xmax, ymin, ymax, xstep;
  QVector<double> x_graph, y_graph;
  QString error_message = "Error: Invalid input expression";
  QChar sym_plus, sym_minus, sym_mult, sym_div, sym_mod, sym_pow, sym_var;

  Ui::MainWindow *ui;
  int height_graph_hidden;
  int height_graph_visible;
  QPropertyAnimation *window_animation = nullptr;

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  bool verifySequenceCorrectness(QString str, QChar sym);
  QString qstr_display_to_internal(QString &display);
  QString qstr_internal_to_display(QString &display);
  bool isOperator(QChar sym);
  bool isGraphOpen();
  void updateGraph();
  void prepareDisplay(QAbstractButton *button);
  void animateWindowSize();

 private slots:
  void on_eq_clicked();
  void on_ac_clicked();
  void on_del_clicked();
  void on_xVal_textChanged(const QString &val);
  void on_symbolButtonGroup_buttonClicked(QAbstractButton *button);
  void on_functionButtonGroup_buttonClicked(QAbstractButton *button);
  void on_plot_clicked();
  void on_xStep_textChanged(const QString &val);
};

#endif  // MainWindow_HPP_
