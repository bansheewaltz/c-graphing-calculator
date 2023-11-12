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

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void from_QStr_to_string(char *str, QString qstr);
  double x, xmax, xMin, ymax, ymin;

 private slots:
  void StartLabel(QAbstractButton *button);

  void setText(QAbstractButton *button);
  void setText_function(QAbstractButton *button);
  void SetpromptX(const QString &arg1);
  void setText_AC();
  void pushButtonCalculate();

  void on_plot_clicked();
  void animateWindowSize();

 private:
  Ui::MainWindow *ui;
  QVector<double> x_graph, y_graph;
  QPropertyAnimation *animation = nullptr;
};
#endif  // MainWindow_HPP_
