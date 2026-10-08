#pragma once

#include <QWidget>
#include <memory>
#include "calculatorengine.h"

namespace Ui { class MainWindow; }

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void handleCommand(const QString &command);
    void updateDisplay();
    std::unique_ptr<Ui::MainWindow> ui;
    CalculatorEngine engine;
};
