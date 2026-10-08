#include "calculatorengine.h"
#include <cmath>

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
        if (m_state == State::Error || m_state == State::Result) {
            *this = CalculatorEngine();
        } else {
            m_entry = "0";
            // CE 只清当前操作数，保留第一个操作数和操作符。
            m_state = m_operator.isNull() ? State::FirstOperand : State::SecondOperand;
        }
    } else if (command == "sign")
        toggleSign();
    else if (command.size() == 1 && QString("+-*/").contains(command.front()))
        inputOperator(command.front());
    else if (command == "equals")
        calculate();
}

void CalculatorEngine::prepareForEntry()
{
    // 结果/错误后的数字和小数点开启新计算，操作符则可接着结果计算。
    if (m_state == State::Result || m_state == State::Error)
        *this = CalculatorEngine();
    else if (m_state == State::OperatorPending) {
        m_entry = "0";
        m_state = State::SecondOperand;
    }
    m_completedExpression.clear();
}

void CalculatorEngine::inputDigit(QChar digit)
{
    prepareForEntry();
    if (m_entry == "0" || m_entry == "-0") {
        m_entry = (m_entry.startsWith('-') ? QString("-") : QString()) + digit;
        return;
    }
    QString digits = m_entry;
    digits.remove('.').remove('-');
    // 限制每个操作数输入 15 位数字，避免精度及显示长度失控。
    if (digits.size() < 15)
        m_entry += digit;
}

void CalculatorEngine::inputDecimal()
{
    prepareForEntry();
    // 每个操作数独立检查；第二次及后续小数点不改变状态。
    if (!m_entry.contains('.'))
        m_entry += '.';
}

void CalculatorEngine::backspace()
{
    // 仅允许编辑正在输入的操作数，不能删除结果、错误或等待状态的显示。
    if (m_state != State::FirstOperand && m_state != State::SecondOperand)
        return;
    m_entry.chop(1);
    if (m_entry.isEmpty() || m_entry == "-")
        m_entry = "0";
}

void CalculatorEngine::toggleSign()
{
    if (m_state == State::Error)
        return;
    if (m_state == State::OperatorPending)
        prepareForEntry();
    m_completedExpression.clear();
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
         + (m_state == State::OperatorPending ? QString() : " " + m_entry);
}

void CalculatorEngine::inputOperator(QChar operation)
{
    if (m_state == State::Error)
        return;
    // 标准计算器按输入顺序计算，2 + 3 × 4 = 20。
    if (m_state == State::SecondOperand) {
        calculate();
        if (m_state == State::Error)
            return;
    }
    // 连续操作符只替换操作符，不把显示内容再次作为第二操作数。
    if (m_state != State::OperatorPending)
        m_firstOperand = m_entry.toDouble();
    m_operator = operation;
    m_state = State::OperatorPending;
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
    // 没有第二操作数时忽略等号；重复等号也不重复执行上一次计算。
    if (m_state != State::SecondOperand || m_operator.isNull())
        return;
    m_completedExpression = expression() + " =";
    const double right = m_entry.toDouble();
    if (m_operator == '/' && right == 0) {
        m_entry = QString::fromUtf8("不能除以零");
        m_operator = QChar();
        m_state = State::Error;
        return;
    }
    const double result = apply(m_firstOperand, right, m_operator);
    m_operator = QChar();
    if (!std::isfinite(result)) {
        m_entry = QString::fromUtf8("结果超出范围");
        m_state = State::Error;
        return;
    }
    m_entry = result == 0 ? QString("0") : QString::number(result, 'g', 15);
    m_state = State::Result;
}
