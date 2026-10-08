#include "calculatorwindow.h"
#include "ui_calculatorwindow.h"

CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QWidget(parent), ui(std::make_unique<Ui::CalculatorWindow>())
{
    ui->setupUi(this);
}

CalculatorWindow::~CalculatorWindow() = default;
