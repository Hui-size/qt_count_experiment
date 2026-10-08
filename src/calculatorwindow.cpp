#include "calculatorwindow.h"
#include "ui_calculatorwindow.h"

CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QWidget(parent), ui(std::make_unique<Ui::CalculatorWindow>())
{
    ui->setupUi(this);
    for (auto *button : findChildren<QPushButton *>()) {
        const QString command = button->property("command").toString();
        connect(button, &QPushButton::clicked, this, [this, command] {
            handleCommand(command);
        });
    }
    updateDisplay();
}

CalculatorWindow::~CalculatorWindow() = default;

void CalculatorWindow::handleCommand(const QString &command)
{
    engine.dispatch(command);
    updateDisplay();
}

void CalculatorWindow::updateDisplay()
{
    ui->displayEdit->setText(engine.display());
    ui->expressionLabel->setText(engine.expression());
}
