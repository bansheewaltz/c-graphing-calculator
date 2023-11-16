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

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void setDisplayPrepare(QAbstractButton *button);
  void setDisplayReset();
  void setDisplayAddSymbol(QAbstractButton *button);
  void setDisplayAddFunction(QAbstractButton *button);

  void setxVal(const QString &val);
  void calculate();

  void on_plot_clicked();
  void animateWindowSize();

 private:
  Ui::MainWindow *ui;
  QVector<double> x_graph, y_graph;
  QPropertyAnimation *window_animation = nullptr;
};

#endif  // MainWindow_HPP_
