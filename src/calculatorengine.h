#pragma once

#include <QString>

// 所有交互都转换为 command，计算状态与窗口控件无关。
class CalculatorEngine
{
public:
    void dispatch(const QString &command);
    QString display() const { return m_entry; }
    QString expression() const { return {}; }

private:
    void inputDigit(QChar digit);
    void inputDecimal();
    void backspace();
    void toggleSign();
    QString m_entry = "0";
};
