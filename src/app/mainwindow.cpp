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
  QString input_qstr = ui->outputDisplay->text();
  for (int i = 0; i < input_qstr.length(); i++) {
    if (input_qstr[i] == QChar(0x00D7)) input_qstr[i] = QChar('*');  // ×
    if (input_qstr[i] == QChar(0x2013)) input_qstr[i] = QChar('-');  // –
  }
  std::string input_str = input_qstr.toStdString();
  const char *input_cstr = input_str.c_str();

  double res;
  SmartCalcError rc;
  rc = smartcalc_expr_infix_evaluate(input_cstr, this->x, &res);
  if (rc == SMARTCALC_SUCCESS) {
    QString output_qstr = QString::number(res, 'g', 7);
    for (int i = 0; i < output_qstr.length(); i++) {
      if (output_qstr[i] == QChar('*')) output_qstr[i] = QChar(0x00D7);  // ×
      if (output_qstr[i] == QChar('-')) output_qstr[i] = QChar(0x2013);  // –
    }
    ui->outputDisplay->setText(output_qstr);
  } else
    ui->outputDisplay->setText("Error: Invalid input expression.");

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
  window_animation = new QPropertyAnimation(this, "size");
  window_animation->setDuration(350);
  connect(window_animation, &QPropertyAnimation::finished, this, [this]() {
    delete window_animation;
    window_animation = nullptr;
  });

  if (window_animation->state() != QAbstractAnimation::Running) {
    if (height() == minimumHeight()) {
      window_animation->setStartValue(QSize(width(), minimumHeight()));
      window_animation->setEndValue(QSize(width(), maximumHeight()));
    } else {
      window_animation->setStartValue(QSize(width(), height()));
      window_animation->setEndValue(QSize(width(), minimumHeight()));
    }
    window_animation->start();
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
