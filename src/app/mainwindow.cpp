#include "mainwindow.hpp"

#include "./ui_mainwindow.h"
#include "smartcalc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setProperty("windowOpacity", 0.98);

  height_graph_hidden = height();
  height_graph_visible = height_graph_hidden * 2;
  setMaximumHeight(height_graph_hidden);

  sym_plus = ui->plus->text().front();
  sym_minus = ui->minus->text().front();
  sym_mult = ui->mult->text().front();
  sym_div = ui->div->text().front();
  sym_mod = ui->mod->text().front();
  sym_pow = ui->pow->text().front();
  sym_var = ui->push_x->text().front();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::prepareDisplay(QAbstractButton *button) {
  if (ui->outputDisplay->text() == "0" && button->text() != "." ||
      ui->outputDisplay->text() == error_message)
    ui->outputDisplay->setText("");
}

void MainWindow::on_xVal_textChanged(const QString &val) { x = val.toDouble(); }

QString MainWindow::qstr_display_to_internal(QString &display) {
  QString internal = display;
  for (int i = 0; i < internal.length(); i++) {
    if (internal[i] == sym_mult) internal[i] = QChar('*');   // ×
    if (internal[i] == sym_minus) internal[i] = QChar('-');  // –
    if (internal[i] == sym_div) internal[i] = QChar('/');    // ÷
    if (internal[i] == sym_mod) {
      internal.removeAt(i);
      internal.insert(i, "mod"), i += 2;
    }
  }
  return internal;
}

QString MainWindow::qstr_internal_to_display(QString &internal) {
  QString display = internal;
  for (int i = 0; i < display.length(); i++) {
    if (display[i] == QChar('*')) display[i] = sym_mult;   // ×
    if (display[i] == QChar('-')) display[i] = sym_minus;  // –
    if (display[i] == QChar('/')) display[i] = sym_div;    // ÷
  }
  return display;
}

bool MainWindow::isOperator(QChar sym) {
  if (sym == sym_plus) return true;
  if (sym == sym_minus) return true;
  if (sym == sym_mult) return true;
  if (sym == sym_div) return true;
  if (sym == sym_mod) return true;
  if (sym == sym_pow) return true;
  return false;
}

bool MainWindow::verifySequenceCorrectness(QString str, QChar sym) {
  if (sym.isDigit()) {
    if (str.back() == ')') return false;
    return true;
  }
  if (isOperator(sym)) {
    if (str == "0") {
      if (sym == sym_minus) return true;
      return false;
    }
    if (isOperator(str.back())) {
      if (str.length() >= 2) {
        QChar last = str.at(str.size() - 1);
        QChar prelast = str.at(str.size() - 2);
        if (isOperator(last) && isOperator(prelast)) return false;
      }
      if (sym == sym_minus) return true;
      return false;
    }
  }
  if (sym == sym_var) {
    if (str.back() == sym_var) return false;
    return true;
  }
  if (sym == '(') return true;
  if (sym == ')') {
    if (str == "0") return false;
    return true;
  }
  if (sym == '.') {
    if (str.back() == sym_var) {
      return false;
    }
    int i = str.length() - 1;
    while (i >= 0 && str.at(i).isDigit()) {
      i--;
    }
    if (i >= 0 && str.at(i) == '.') return false;
  }
  return true;
}

void MainWindow::on_eq_clicked() {
  ui->outputDisplay->setFocus();
  QString input_qstr = ui->outputDisplay->text();
  QString input_qstr_fmt = qstr_display_to_internal(input_qstr);
  std::string input_str = input_qstr_fmt.toStdString();
  const char *input_cstr = input_str.c_str();

  double res;
  SmartCalcError rc;
  rc = smartcalc_expr_infix_evaluate(input_cstr, this->x, &res);
  if (rc == SMARTCALC_ERR_SUCCESS) {
    QString display_qstr = QString::number(res, 'g', 16);
    QString display_qstr_fmt = qstr_internal_to_display(display_qstr);
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
  if (isGraphOpen()) {
    updateGraph();
  }
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
  if (str.isEmpty()) return;

  str.removeLast();

  while (!str.isEmpty() && str.back().isLetter()) {
    if (str.back() == sym_var) break;
    str.removeLast();
  }

  if (str.isEmpty())
    ui->outputDisplay->setText("0");
  else
    ui->outputDisplay->setText(str);

  updateGraph();
}

void MainWindow::animateWindowSize() {
  window_animation = new QPropertyAnimation(this, "size");
  window_animation->setDuration(350);
  bool shrink_height = (height() != height_graph_hidden);
  connect(window_animation, &QPropertyAnimation::finished, this,
          [this, shrink_height]() {
            delete window_animation;
            window_animation = nullptr;
            if (shrink_height) setMaximumHeight(height_graph_hidden);
          });

  if (window_animation->state() != QAbstractAnimation::Running) {
    if (height() == height_graph_hidden) {
      setMaximumHeight(height_graph_visible);
      window_animation->setStartValue(QSize(width(), height()));
      window_animation->setEndValue(QSize(width(), height_graph_visible));
    } else {
      window_animation->setStartValue(QSize(width(), height()));
      window_animation->setEndValue(QSize(width(), height_graph_hidden));
    }
    window_animation->start();
  }
}

void MainWindow::on_plot_clicked() {
  animateWindowSize();
  updateGraph();
}

bool MainWindow::isGraphOpen() {
  return this->height() != this->minimumHeight();
}

void MainWindow::updateGraph() {
  if (isOperator(ui->outputDisplay->text().back())) {
    return;
  }

  xmin = ui->xMin->text().toDouble();
  xmax = ui->xMax->text().toDouble();
  ymin = ui->yMin->text().toDouble();
  ymax = ui->yMax->text().toDouble();
  xstep = ui->xStep->text().toDouble();
  if (xstep < 0.001) xstep = 0.001;

  if (xmin > xmax || ymin > ymax) {
    ui->outputDisplay->setText("Error: invalid function ranges");
    return;
  }

  if (ui->xMin->text().isEmpty()) xmin = -100;
  if (ui->xMax->text().isEmpty()) xmax = +100;
  if (ui->yMin->text().isEmpty()) ymin = -100;
  if (ui->yMax->text().isEmpty()) ymax = +100;
  if (ui->xStep->text().isEmpty()) xstep = +0.1;

  ui->graphWidget->xAxis->setRange(xmin, xmax);
  ui->graphWidget->yAxis->setRange(ymin, ymax);
  ui->graphWidget->setInteraction(QCP::iRangeDrag);
  ui->graphWidget->setInteraction(QCP::iRangeZoom);

  QString expr_qstr = ui->outputDisplay->text();
  QString expr_qstr_fmt = qstr_display_to_internal(expr_qstr);
  std::string expr_str = expr_qstr_fmt.toStdString();
  const char *expr_cstr = expr_str.c_str();

  double res;
  SmartCalcError rc;

  rc = smartcalc_expr_infix_evaluate(expr_cstr, this->x, &res);
  if (rc == SMARTCALC_ERR_SUCCESS) {
    for (double x = xmin; x <= xmax; x += xstep) {
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

void MainWindow::on_xStep_textChanged(const QString &val) {
  xstep = val.toDouble();
  updateGraph();
}
