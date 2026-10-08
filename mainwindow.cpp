#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(std::make_unique<Ui::MainWindow>())
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

MainWindow::~MainWindow() = default;

void MainWindow::handleCommand(const QString &command)
{
    engine.dispatch(command);
    updateDisplay();
}

void MainWindow::updateDisplay()
{
    ui->displayEdit->setText(engine.display());
    ui->expressionLabel->setText(engine.expression());
}
