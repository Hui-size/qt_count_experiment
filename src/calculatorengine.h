#pragma once

#include <QString>

// 所有交互都转换为 command，计算状态与窗口控件无关。
class CalculatorEngine
{
public:
    void dispatch(const QString &command);
    QString display() const { return m_entry; }
    QString expression() const;

private:
    void inputDigit(QChar digit);
    void inputDecimal();
    void backspace();
    void toggleSign();
    void inputOperator(QChar operation);
    void calculate();
    double apply(double left, double right, QChar operation) const;
    QString m_entry = "0";
    double m_firstOperand = 0;
    QChar m_operator;
    QString m_completedExpression;
    bool m_waitingForOperand = false;
};
