#pragma once

#include <QWidget>
#include <memory>
#include "calculatorengine.h"

namespace Ui { class CalculatorWindow; }

class CalculatorWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CalculatorWindow(QWidget *parent = nullptr);
    ~CalculatorWindow() override;

private:
    void handleCommand(const QString &command);
    void updateDisplay();
    std::unique_ptr<Ui::CalculatorWindow> ui;
    CalculatorEngine engine;
};
