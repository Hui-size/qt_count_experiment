#include "calculatorwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTest>

class CalculatorUiTest : public QObject
{
    Q_OBJECT
private slots:
    void initialInterface()
    {
        CalculatorWindow window;
        auto *display = window.findChild<QLineEdit *>("displayEdit");
        QVERIFY(display);
        QCOMPARE(display->text(), QString("0"));
        QVERIFY(display->isReadOnly());
        QCOMPARE(window.findChildren<QPushButton *>().size(), 20);
        QVERIFY(window.layout());
    }
};

QTEST_MAIN(CalculatorUiTest)
#include "test_calculator.moc"
