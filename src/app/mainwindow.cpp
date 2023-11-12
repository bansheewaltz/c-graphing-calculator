#include "mainwindow.hpp"

#include "./ui_mainwindow.h"
#include "smartcalc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->outputLabel->setFocus();
  this->setProperty("windowOpacity", 0.98);

  connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this,
          &MainWindow::setText);
  connect(ui->buttonGroup_function, &QButtonGroup::buttonClicked, this,
          &MainWindow::setText_function);
  connect(ui->AC, &QPushButton::clicked, this, &MainWindow::setText_AC);
  connect(ui->eq, &QPushButton::clicked, this,
          &MainWindow::pushButtonCalculate);

  connect(ui->promptX, &QLineEdit::textChanged, this, &MainWindow::SetpromptX);
  ui->graphWidget->setVisible(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::StartLabel(QAbstractButton *button) {
  if (ui->outputLabel->text() == "0" && button->text() != ".")
    ui->outputLabel->setText("");
  if (ui->outputLabel->text() == "Error: Invalid input expression." ||
      ui->outputLabel->text() == "Error: Error in calculation.")
    ui->outputLabel->setText("");
}

void MainWindow::SetpromptX(const QString &arg1) { x = arg1.toDouble(); }

void MainWindow::pushButtonCalculate() {
  std::string inputStringStr = ui->outputLabel->text().toStdString();
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

  ui->promptX->setText("");
}

void MainWindow::setText(QAbstractButton *button) {
  StartLabel(button);
  if (button->text() != "÷")
    ui->outputLabel->setText(ui->outputLabel->text() + button->text());
  else
    ui->outputLabel->setText(ui->outputLabel->text() + '/');
}

void MainWindow::setText_function(QAbstractButton *button) {
  StartLabel(button);
  ui->outputLabel->setText(ui->outputLabel->text() + button->text() + "(");
}

void MainWindow::setText_AC() {
  ui->outputLabel->setText("0");

  ui->promptX->setText("");
  ui->xMin->setText("");
  ui->xMax->setText("");
  ui->yMin->setText("");
  ui->yMax->setText("");

  ui->outputLabel->setFocus();
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
      ui->outputLabel->text() == "0") {
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

    std::string inputStringStr = ui->outputLabel->text().toStdString();
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
