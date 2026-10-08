#pragma once

#include <QWidget>
#include <memory>
#include "calculatorengine.h"

namespace Ui { class MainWindow; }
class QKeyEvent;
class QResizeEvent;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    static QString commandForKey(const QKeyEvent &event);
    void fitDisplayFont();
    void handleCommand(const QString &command);
    void updateDisplay();
    std::unique_ptr<Ui::MainWindow> ui;
    CalculatorEngine engine;
};
