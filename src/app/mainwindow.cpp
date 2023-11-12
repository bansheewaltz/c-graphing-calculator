#include "mainwindow.hpp"

#include "./ui_mainwindow.h"
#include "smartcalc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setProperty("windowOpacity", 0.98);

  connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this,
          &MainWindow::setDisplayAddSymbol);
  connect(ui->buttonGroup_function, &QButtonGroup::buttonClicked, this,
          &MainWindow::setDisplayAddFunction);
  connect(ui->ac, &QPushButton::clicked, this, &MainWindow::setDisplayReset);

  connect(ui->eq, &QPushButton::clicked, this, &MainWindow::calculate);
  connect(ui->xVal, &QLineEdit::textChanged, this, &MainWindow::setxVal);

  ui->graphWidget->setVisible(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setDisplayPrepare(QAbstractButton *button) {
  if (ui->outputDisplay->text() == "0" && button->text() != "." ||
      ui->outputDisplay->text() == "Error: Invalid input expression." ||
      ui->outputDisplay->text() == "Error: Error in calculation.")
    ui->outputDisplay->setText("");
}

void MainWindow::setxVal(const QString &val) { x = val.toDouble(); }

void MainWindow::calculate() {
  std::string inputStringStr = ui->outputDisplay->text().toStdString();
  const char *inputStr = inputStringStr.c_str();

  char postfixStr[255] = {0};
  double outputStr = 0;

  //  if (parser(inputStr, postfixStr)) {
  //    ui->label->setText("Error: Invalid input expression.");
  //  } else {
  //    if (calculation(postfixStr, &outputStr, x)) {
  //      ui->label->setText("Error: Error in calculation.");
  //    } else {
  //      ui->label->setText(QString::number(outputStr, 'g', 16));  // x - error
  //      1
  //    }
  //  }

  ui->xVal->setText("");
}

void MainWindow::setDisplayAddSymbol(QAbstractButton *button) {
  setDisplayPrepare(button);
  if (button->text() != "÷")
    ui->outputDisplay->setText(ui->outputDisplay->text() + button->text());
  else
    ui->outputDisplay->setText(ui->outputDisplay->text() + '/');
}

void MainWindow::setDisplayAddFunction(QAbstractButton *button) {
  setDisplayPrepare(button);
  ui->outputDisplay->setText(ui->outputDisplay->text() + button->text() + "(");
}

void MainWindow::setDisplayReset() {
  ui->outputDisplay->setText("0");
  ui->xVal->setText("");
  ui->xMin->setText("");
  ui->xMax->setText("");
  ui->yMin->setText("");
  ui->yMax->setText("");
}

void MainWindow::animateWindowSize() {
  if (!animation) {
    animation = new QPropertyAnimation(this, "size");
    animation->setDuration(350);

    connect(animation, &QPropertyAnimation::finished, this, [this]() {
      delete animation;
      animation = nullptr;
    });
  }

  if (animation->state() != QAbstractAnimation::Running &&
      ui->outputDisplay->text() == "0") {
    if (width() == minimumWidth()) {
      animation->setStartValue(QSize(minimumWidth(), height()));
      animation->setEndValue(QSize(maximumWidth(), height()));
    } else if (width() == maximumWidth()) {
      animation->setStartValue(QSize(maximumWidth(), height()));
      animation->setEndValue(QSize(minimumWidth(), height()));
    }

    animation->start();
  }
}

void MainWindow::on_plot_clicked() {
  animateWindowSize();

  if (width() == maximumWidth()) {
    xmin = ui->xMin->text().toDouble();
    xmax = ui->xMax->text().toDouble();
    ymin = ui->yMin->text().toDouble();
    ymax = ui->yMax->text().toDouble();

    if (ui->xMin->text().isEmpty()) xmin = -100;
    if (ui->xMax->text().isEmpty()) xmax = +100;
    if (ui->yMin->text().isEmpty()) ymin = -100;
    if (ui->yMax->text().isEmpty()) ymax = +100;

    ui->graphWidget->xAxis->setRange(xmin, xmax);
    ui->graphWidget->yAxis->setRange(ymin, ymax);
    ui->graphWidget->setInteraction(QCP::iRangeDrag);
    ui->graphWidget->setInteraction(QCP::iRangeZoom);

    std::string inputStringStr = ui->outputDisplay->text().toStdString();
    const char *inputStr = inputStringStr.c_str();

    char postfixStr[255] = {0};

    //    if (parser(inputStr, postfixStr)) {
    //      ui->label->setText("Error: Invalid input expression.");
    //    } else {
    //      if ((xMin <= xmax) && (ymin <= ymax)) {
    //        double output = 0;

    //        for (double X = xMin; X <= xmax; X += 0.1) {
    //          x_graph.push_back(X);
    //          calculation(postfixStr, &output, X);
    //          if (ymin <= output && output <= ymax) {
    //            y_graph.push_back(output);
    //          } else {
    //            y_graph.push_back(qQNaN());
    //          }
    //        }

    //        ui->graphWidget->addGraph();
    //        ui->graphWidget->graph(0)->setData(x_graph, y_graph);
    //        ui->graphWidget->replot();
    //        x_graph.clear();
    //        y_graph.clear();

    //      } else {
    //        ui->label->setText("Error: Invalid input expression.");
    //      }
    //    }
  }
}
