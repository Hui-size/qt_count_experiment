#pragma once

#include <QString>

// 所有交互都转换为 command，计算状态与窗口控件无关。
class CalculatorEngine
{
public:
    enum class State { FirstOperand, OperatorPending, SecondOperand, Result, Error };
    void dispatch(const QString &command);
    QString display() const { return m_entry; }
    QString expression() const;
    State state() const { return m_state; }
    bool hasError() const { return m_state == State::Error; }
    QChar pendingOperator() const { return m_operator; }

private:
    void prepareForEntry();
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
    State m_state = State::FirstOperand;
};
