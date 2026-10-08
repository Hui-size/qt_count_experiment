#include "calculatorengine.h"

void CalculatorEngine::dispatch(const QString &command)
{
    if (command.size() == 1 && command.front() >= '0' && command.front() <= '9')
        inputDigit(command.front());
    else if (command == "decimal")
        inputDecimal();
    else if (command == "backspace")
        backspace();
    else if (command == "clear")
        *this = CalculatorEngine();
    else if (command == "clearEntry") {
        m_entry = "0";
        m_waitingForOperand = false;
    }
    else if (command == "sign")
        toggleSign();
    else if (command.size() == 1 && QString("+-*/").contains(command.front()))
        inputOperator(command.front());
    else if (command == "equals")
        calculate();
}

void CalculatorEngine::inputDigit(QChar digit)
{
    if (m_waitingForOperand) {
        m_entry = "0";
        m_waitingForOperand = false;
    }
    m_completedExpression.clear();
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
    if (m_waitingForOperand) {
        m_entry = "0";
        m_waitingForOperand = false;
    }
    m_completedExpression.clear();
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

QString CalculatorEngine::expression() const
{
    if (!m_completedExpression.isEmpty())
        return m_completedExpression;
    if (m_operator.isNull())
        return {};
    const QString symbol = m_operator == '*' ? QString::fromUtf8("×")
                         : m_operator == '/' ? QString::fromUtf8("÷")
                         : QString(m_operator);
    return QString::number(m_firstOperand, 'g', 15) + " " + symbol
         + (m_waitingForOperand ? QString() : " " + m_entry);
}

void CalculatorEngine::inputOperator(QChar operation)
{
    // 按标准计算器顺序计算：2 + 3 × 4 得到 20。
    if (!m_operator.isNull() && !m_waitingForOperand)
        calculate();
    m_firstOperand = m_entry.toDouble();
    m_operator = operation;
    m_waitingForOperand = true;
    m_completedExpression.clear();
}

double CalculatorEngine::apply(double left, double right, QChar operation) const
{
    if (operation == '+') return left + right;
    if (operation == '-') return left - right;
    if (operation == '*') return left * right;
    return left / right;
}

void CalculatorEngine::calculate()
{
    if (m_operator.isNull() || m_waitingForOperand)
        return;
    m_completedExpression = expression() + " =";
    m_entry = QString::number(apply(m_firstOperand, m_entry.toDouble(), m_operator), 'g', 15);
    m_operator = QChar();
    m_waitingForOperand = false;
}
