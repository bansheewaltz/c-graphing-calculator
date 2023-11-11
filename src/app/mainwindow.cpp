#include "mainwindow.hpp"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->label->setFocus();
  this->setProperty("windowOpacity", 0.98);

  connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this,
          &MainWindow::setText);
  connect(ui->buttonGroup_function, &QButtonGroup::buttonClicked, this,
          &MainWindow::setText_function);
  connect(ui->AC, &QPushButton::clicked, this, &MainWindow::setText_AC);
  connect(ui->eq, &QPushButton::clicked, this,
          &MainWindow::pushButtonCalculate);

  connect(ui->EnterX, &QLineEdit::textChanged, this, &MainWindow::SetEnterX);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::StartLabel(QAbstractButton *button) {
  if (ui->label->text() == "0" && button->text() != ".") ui->label->setText("");
  if (ui->label->text() == "Error: Invalid input expression." ||
      ui->label->text() == "Error: Error in calculation.")
    ui->label->setText("");
}

void MainWindow::SetEnterX(const QString &arg1) { x = arg1.toDouble(); }

void MainWindow::pushButtonCalculate() {
  std::string inputStringStr = ui->label->text().toStdString();
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

  ui->EnterX->setText("");
}

void MainWindow::setText(QAbstractButton *button) {
  StartLabel(button);
  if (button->text() != "÷")
    ui->label->setText(ui->label->text() + button->text());
  else
    ui->label->setText(ui->label->text() + '/');
}

void MainWindow::setText_function(QAbstractButton *button) {
  StartLabel(button);
  ui->label->setText(ui->label->text() + button->text() + "(");
}

void MainWindow::setText_AC() {
  ui->label->setText("0");

  ui->EnterX->setText("");
  ui->Xmax->setText("");
  ui->Xmin->setText("");
  ui->Ymax->setText("");
  ui->Ymin->setText("");

  ui->label->setFocus();
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
      ui->label->text() == "0") {
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

void MainWindow::on_Graph_clicked() {
  animateWindowSize();

  if (width() == maximumWidth()) {
    xmax = ui->Xmax->text().toDouble();
    xmin = ui->Xmin->text().toDouble();
    ymax = ui->Ymax->text().toDouble();
    ymin = ui->Ymin->text().toDouble();

    if (ui->Xmax->text().isEmpty()) {
      xmax = 100;
    }
    if (ui->Xmin->text().isEmpty()) {
      xmin = -100;
    }
    if (ui->Ymax->text().isEmpty()) {
      ymax = 100;
    }
    if (ui->Ymin->text().isEmpty()) {
      ymin = -100;
    }

    ui->widget->xAxis->setRange(xmin, xmax);
    ui->widget->yAxis->setRange(ymin, ymax);
    ui->widget->setInteraction(QCP::iRangeDrag);
    ui->widget->setInteraction(QCP::iRangeZoom);

    std::string inputStringStr = ui->label->text().toStdString();
    const char *inputStr = inputStringStr.c_str();

    char postfixStr[255] = {0};

    //    if (parser(inputStr, postfixStr)) {
    //      ui->label->setText("Error: Invalid input expression.");
    //    } else {
    //      if ((xmin <= xmax) && (ymin <= ymax)) {
    //        double output = 0;

    //        for (double X = xmin; X <= xmax; X += 0.1) {
    //          x_graph.push_back(X);
    //          calculation(postfixStr, &output, X);
    //          if (ymin <= output && output <= ymax) {
    //            y_graph.push_back(output);
    //          } else {
    //            y_graph.push_back(qQNaN());
    //          }
    //        }

    //        ui->widget->addGraph();
    //        ui->widget->graph(0)->setData(x_graph, y_graph);
    //        ui->widget->replot();
    //        x_graph.clear();
    //        y_graph.clear();

    //      } else {
    //        ui->label->setText("Error: Invalid input expression.");
    //      }
    //    }
  }
}
