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

    void mouseEntry()
    {
        CalculatorWindow window;
        for (const QString &name : {"oneButton", "decimalButton", "decimalButton", "twoButton"}) {
            auto *button = window.findChild<QPushButton *>(name);
            QVERIFY(button);
            QTest::mouseClick(button, Qt::LeftButton);
        }
        QCOMPARE(window.findChild<QLineEdit *>("displayEdit")->text(), QString("1.2"));
        QTest::mouseClick(window.findChild<QPushButton *>("clearButton"), Qt::LeftButton);
        QCOMPARE(window.findChild<QLineEdit *>("displayEdit")->text(), QString("0"));
    }

    void mouseCalculation()
    {
        CalculatorWindow window;
        for (const QString &name : {"oneButton", "decimalButton", "fiveButton", "addButton", "twoButton", "equalsButton"})
            QTest::mouseClick(window.findChild<QPushButton *>(name), Qt::LeftButton);
        QCOMPARE(window.findChild<QLineEdit *>("displayEdit")->text(), QString("3.5"));
    }
};

QTEST_MAIN(CalculatorUiTest)
#include "test_calculator.moc"
