#include "calculatorengine.h"

void CalculatorEngine::dispatch(const QString &command)
{
    if (command.size() == 1 && command.front() >= '0' && command.front() <= '9')
        inputDigit(command.front());
    else if (command == "decimal")
        inputDecimal();
    else if (command == "backspace")
        backspace();
    else if (command == "clear" || command == "clearEntry")
        m_entry = "0";
    else if (command == "sign")
        toggleSign();
}

void CalculatorEngine::inputDigit(QChar digit)
{
    if (m_entry == "0" || m_entry == "-0") {
        m_entry = (m_entry.startsWith('-') ? QString("-") : QString()) + digit;
        return;
    }
    QString digits = m_entry;
    digits.remove('.').remove('-');
    // 限制输入为 15 位有效数字，避免 double 输入精度和界面长度失控。
    if (digits.size() < 15)
        m_entry += digit;
}

void CalculatorEngine::inputDecimal()
{
    // 当前操作数已经有小数点时忽略重复输入。
    if (!m_entry.contains('.'))
        m_entry += '.';
}

void CalculatorEngine::backspace()
{
    m_entry.chop(1);
    if (m_entry.isEmpty() || m_entry == "-")
        m_entry = "0";
}

void CalculatorEngine::toggleSign()
{
    if (m_entry.startsWith('-'))
        m_entry.remove(0, 1);
    else if (m_entry.toDouble() != 0)
        m_entry.prepend('-');
}
