#include "mainwindow.hpp"

#include "./ui_mainwindow.h"
#include "smartcalc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setProperty("windowOpacity", 0.98);
  op_symbols_lut.resize(Operators::ENUM_SIZE);
  op_symbols_lut[Operators::SUM] = ui->plus->text().front();
  op_symbols_lut[Operators::SUB] = ui->minus->text().front();
  op_symbols_lut[Operators::MUL] = ui->mult->text().front();
  op_symbols_lut[Operators::DIV] = ui->div->text().front();
  op_symbols_lut[Operators::MOD] = ui->mod->text().front();
  op_symbols_lut[Operators::POW] = ui->pow->text().front();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::prepareDisplay(QAbstractButton *button) {
  if (ui->outputDisplay->text() == "0" && button->text() != "." ||
      ui->outputDisplay->text() == error_message)
    ui->outputDisplay->setText("");
}

void MainWindow::on_xVal_textChanged(const QString &arg1) {
  x = arg1.toDouble();
}

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
}  // namespace SmartCalc

bool MainWindow::isOperator(QChar sym) {
  for (int i = 0; i < op_symbols_lut.size(); i++)
    if (op_symbols_lut[i] == sym) return true;
  return false;
}

bool MainWindow::verifySequenceCorrectness(QString str, QChar sym) {
  if (sym.isDigit()) {
    if (str.back() == ')') return false;
    return true;
  }
  if (isOperator(sym)) {
    if (isOperator(str.back())) {
      if (str.length() >= 2) {
        QChar last = str.at(str.size() - 1);
        QChar prelast = str.at(str.size() - 2);
        if (isOperator(last) && isOperator(prelast)) return false;
      }
      if (sym == op_symbols_lut[Operators::SUB]) return true;
      return false;
    }
  }
  if (sym == 'x') {
    if (str.back() == 'x') return false;
    return true;
  }
  if (sym == '(') return true;
  if (sym == ')') {
    if (str == "0") return false;
    return true;
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
  return true;
}

void MainWindow::on_eq_clicked() {
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
    ui->outputDisplay->setText(error_message);

  ui->xVal->setText("");
}

void MainWindow::on_symbolButtonGroup_buttonClicked(QAbstractButton *button) {
  QString display = ui->outputDisplay->text();
  QChar symbol = button->text().front();
  bool correct = verifySequenceCorrectness(display, symbol);
  if (!correct) return;
  prepareDisplay(button);
  ui->outputDisplay->setText(ui->outputDisplay->text() + button->text());
  //  if (isGraphOpen()) {
  if (!isOperator(ui->outputDisplay->text().back())) {
    updateGraph();
  }
  //  }
}

void MainWindow::on_functionButtonGroup_buttonClicked(QAbstractButton *button) {
  prepareDisplay(button);
  ui->outputDisplay->setText(ui->outputDisplay->text() + button->text() + "(");
}

void MainWindow::on_ac_clicked() {
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

void MainWindow::on_plot_clicked() { animateWindowSize(); }

bool MainWindow::isGraphOpen() {
  return this->height() != this->minimumHeight();
}

void MainWindow::updateGraph() {
  //  if (!isGraphOpen()) {
  //    return;
  //  }

  xmin = ui->xMin->text().toDouble();
  xmax = ui->xMax->text().toDouble();
  ymin = ui->yMin->text().toDouble();
  ymax = ui->yMax->text().toDouble();

  if (xmin > xmax || ymin > ymax) {
    ui->outputDisplay->setText("Error: invalid function ranges");
    return;
  }

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
