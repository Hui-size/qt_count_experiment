#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEvent>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QStyle>

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
    // 按键无焦点，鼠标点击后键盘继续工作；过滤子控件事件防止显示框吞键。
    for (auto *widget : findChildren<QWidget *>())
        widget->installEventFilter(this);
    updateDisplay();
}

MainWindow::~MainWindow() = default;

void MainWindow::handleCommand(const QString &command)
{
    engine.dispatch(command);
    updateDisplay();
    setFocus(Qt::OtherFocusReason);
}

void MainWindow::updateDisplay()
{
    ui->displayEdit->setText(engine.display());
    ui->expressionLabel->setText(engine.expression());
    ui->displayEdit->setProperty("error", engine.hasError());
    ui->displayEdit->style()->unpolish(ui->displayEdit);
    ui->displayEdit->style()->polish(ui->displayEdit);
    for (auto *button : findChildren<QPushButton *>()) {
        const QString command = button->property("command").toString();
        const bool active = !engine.pendingOperator().isNull()
                         && command == QString(engine.pendingOperator());
        if (button->property("active").toBool() != active) {
            button->setProperty("active", active);
            button->style()->unpolish(button);
            button->style()->polish(button);
        }
    }
    fitDisplayFont();
}

QString MainWindow::commandForKey(const QKeyEvent &event)
{
    // Ctrl/Alt/Meta 组合键不作为计算输入；Shift 用于 +、* 等字符。
    if (event.modifiers() & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier))
        return {};
    switch (event.key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Equal: return "equals";
    case Qt::Key_Backspace: return "backspace";
    case Qt::Key_Escape: return "clear";
    case Qt::Key_Delete: return "clearEntry";
    default: break;
    }
    const QString text = event.text();
    if (text.size() == 1 && text.front() >= '0' && text.front() <= '9')
        return text;
    if (text == "." || text == ",") return "decimal";
    if (text.size() == 1 && QString("+-*/").contains(text.front())) return text;
    if (text.compare("c", Qt::CaseInsensitive) == 0) return "clear";
    return {};
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const QString command = commandForKey(*event);
    if (!command.isEmpty()) {
        handleCommand(command);
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        const QString command = commandForKey(*keyEvent);
        if (!command.isEmpty()) {
            // 与按钮 clicked 信号完全复用同一入口。
            handleCommand(command);
            event->accept();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fitDisplayFont();
}

void MainWindow::fitDisplayFont()
{
    QFont font = ui->displayEdit->font();
    const int availableWidth = qMax(1, ui->displayEdit->contentsRect().width() - 24);
    int pixels = 52;
    font.setPixelSize(pixels);
    while (pixels > 14 && QFontMetrics(font).horizontalAdvance(ui->displayEdit->text()) > availableWidth)
        font.setPixelSize(--pixels);
    ui->displayEdit->setFont(font);
}
