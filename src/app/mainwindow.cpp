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

namespace SmartCalc {
static QString qstr_display_to_internal(QString &display) {
  QString internal = display;
  for (int i = 0; i < internal.length(); i++) {
    if (internal[i] == QChar(0x00D7)) internal[i] = QChar('*');  // ×
    if (internal[i] == QChar(0x2013)) internal[i] = QChar('-');  // –
    if (internal[i] == QChar(0x00F7)) internal[i] = QChar('/');  // ÷
    if (internal[i] == QChar('%')) {
      internal.removeAt(i);
      internal.insert(i, "mod"), i += 2;
    }
  }
  return internal;
}
static QString qstr_internal_to_display(QString &internal) {
  QString display = internal;
  for (int i = 0; i < display.length(); i++) {
    if (display[i] == QChar('*')) display[i] = QChar(0x00D7);  // ×
    if (display[i] == QChar('-')) display[i] = QChar(0x2013);  // –
    if (display[i] == QChar('/')) display[i] = QChar(0x00F7);  // ÷
  }
  return display;
}
bool verifySequenceCorrectness(QString str, QChar sym) {
  if (sym.isDigit()) {
    if (str.back() == ')') return false;
    return true;
  }
  if (str.back() == ')') return true;
  if (sym == 'x') {
    if (str.back() == 'x') return false;
    return true;
  }
  if (sym == '(') return true;
  if (sym == ')') {
    if (str == "0") return false;
    return true;
  }
  if (str == "0") {
    if (sym == QChar(0x2013)) return true;
    if (sym == '.') return true;
    if (sym.isDigit()) return true;
  }
  if (sym == QChar(0x2013)) {
    if (str.back() == 'x') return true;
  }
  if (sym == '.') {
    if (str.back() == 'x') {
      return false;
    }
    int i = str.length() - 1;
    while (str.at(i).isDigit()) {
      i--;
    }
    if (str.at(i) == '.') return false;
  }
  if (!str.back().isDigit()) {  // is operator
    if (sym.isDigit())
      return true;
    else
      return false;
  }
  return true;
}
}  // namespace SmartCalc

void MainWindow::calculate() {
  ui->outputDisplay->setFocus();
  QString input_qstr = ui->outputDisplay->text();
  QString input_qstr_fmt = SmartCalc::qstr_display_to_internal(input_qstr);
  std::string input_str = input_qstr_fmt.toStdString();
  const char *input_cstr = input_str.c_str();

  double res;
  SmartCalcError rc;
  rc = smartcalc_expr_infix_evaluate(input_cstr, this->x, &res);
  if (rc == SMARTCALC_ERR_SUCCESS) {
    QString display_qstr = QString::number(res, 'g', 16);
    QString display_qstr_fmt =
        SmartCalc::qstr_internal_to_display(display_qstr);
    ui->outputDisplay->setText(display_qstr_fmt);
  } else
    ui->outputDisplay->setText("Error: Invalid input expression.");

  ui->xVal->setText("");
}

void MainWindow::setDisplayAddSymbol(QAbstractButton *button) {
  QString display = ui->outputDisplay->text();
  QChar symbol = button->text().front();
  bool correct = SmartCalc::verifySequenceCorrectness(display, symbol);
  if (!correct) return;
  setDisplayPrepare(button);
  ui->outputDisplay->setText(ui->outputDisplay->text() + button->text());
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

void MainWindow::on_del_clicked() {
  QString str = ui->outputDisplay->displayText();
  if (str.length() == 0) return;

  str.removeLast();
  if (str.length() != 0) {
    while (str.back().isLetter()) {
      str.removeLast();
    }
  }

  if (str.length() == 0)
    ui->outputDisplay->setText("0");
  else
    ui->outputDisplay->setText(str);
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
  if (width() != maximumWidth()) {
    return;
  }

  xmin = ui->xMin->text().toDouble();
  xmax = ui->xMax->text().toDouble();
  ymin = ui->yMin->text().toDouble();
  ymax = ui->yMax->text().toDouble();

  if (xmin > xmax || ymin > ymax) {
    ui->outputDisplay->setText("Error: invalid function ranges");
    return;
  }

  animateWindowSize();

  if (ui->xMin->text().isEmpty()) xmin = -100;
  if (ui->xMax->text().isEmpty()) xmax = +100;
  if (ui->yMin->text().isEmpty()) ymin = -100;
  if (ui->yMax->text().isEmpty()) ymax = +100;

  ui->graphWidget->xAxis->setRange(xmin, xmax);
  ui->graphWidget->yAxis->setRange(ymin, ymax);
  ui->graphWidget->setInteraction(QCP::iRangeDrag);
  ui->graphWidget->setInteraction(QCP::iRangeZoom);

  QString expr_qstr = ui->outputDisplay->text();
  QString expr_qstr_fmt = SmartCalc::qstr_display_to_internal(expr_qstr);
  std::string expr_str = expr_qstr_fmt.toStdString();
  const char *expr_cstr = expr_str.c_str();

  double res;
  SmartCalcError rc;

  rc = smartcalc_expr_infix_evaluate(expr_cstr, this->x, &res);
  if (rc == SMARTCALC_ERR_SUCCESS) {
    for (double x = xmin; x <= xmax; x += 0.1) {
      x_graph.push_back(x);
      smartcalc_expr_infix_evaluate(expr_cstr, x, &res);
      if (ymin <= res && res <= ymax) {
        y_graph.push_back(res);
      } else {
        y_graph.push_back(qQNaN());
      }
    }

    ui->graphWidget->addGraph();
    ui->graphWidget->graph(0)->setData(x_graph, y_graph);
    ui->graphWidget->replot();
    x_graph.clear();
    y_graph.clear();
  }
}
