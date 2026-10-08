#include "mainwindow.h"
#include <QLineEdit>
#include <QLabel>
#include <QDir>
#include <QKeyEvent>
#include <QPushButton>
#include <QTest>

class CalculatorUiTest : public QObject
{
    Q_OBJECT
private slots:
    void initialInterface()
    {
        MainWindow window;
        auto *display = window.findChild<QLineEdit *>("displayEdit");
        QVERIFY(display);
        QCOMPARE(display->text(), QString("0"));
        QVERIFY(display->isReadOnly());
        QCOMPARE(window.findChildren<QPushButton *>().size(), 20);
        QVERIFY(window.layout());
    }

    void mouseEntry()
    {
        MainWindow window;
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
        MainWindow window;
        for (const QString &name : {"oneButton", "decimalButton", "fiveButton", "addButton", "twoButton", "equalsButton"})
            QTest::mouseClick(window.findChild<QPushButton *>(name), Qt::LeftButton);
        QCOMPARE(window.findChild<QLineEdit *>("displayEdit")->text(), QString("3.5"));
        const QByteArray previewPath = qgetenv("CALCULATOR_PREVIEW_PATH");
        if (!previewPath.isEmpty()) {
            window.show();
            QTest::qWait(30);
            QVERIFY(window.grab().save(QString::fromLocal8Bit(previewPath)));
        }
    }

    void keyboardSequences_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected");
        QTest::newRow("decimal-add") << "12.5+7.25=" << "19.75";
        QTest::newRow("subtract") << "2-5=" << "-3";
        QTest::newRow("multiply") << "1.5*4=" << "6";
        QTest::newRow("divide") << "9/4=" << "2.25";
        QTest::newRow("duplicate-decimal") << "1..2+3.4=" << "4.6";
        QTest::newRow("replace-operator") << "8+*2=" << "16";
        QTest::newRow("continue-sequentially") << "2+3*4=" << "20";
        QTest::newRow("result-new-number") << "2+3=7" << "7";
        QTest::newRow("error-recovery") << "8/0=2+3=" << "5";
        QTest::newRow("comma-decimal") << "0,5*4=" << "2";
    }

    void keyboardSequences()
    {
        QFETCH(QString, input);
        QFETCH(QString, expected);
        MainWindow window;
        QTest::keyClicks(&window, input);
        QCOMPARE(window.findChild<QLineEdit *>("displayEdit")->text(), expected);
    }

    void mixedInputAndEditing()
    {
        MainWindow window;
        auto *display = window.findChild<QLineEdit *>("displayEdit");
        QTest::mouseClick(window.findChild<QPushButton *>("oneButton"), Qt::LeftButton);
        QTest::keyClicks(&window, "2.5+");
        QTest::mouseClick(window.findChild<QPushButton *>("threeButton"), Qt::LeftButton);
        QTest::keyClicks(&window, ".4");
        QTest::keyClick(&window, Qt::Key_Backspace);
        QTest::keyClicks(&window, "5");
        QTest::keyClick(&window, Qt::Key_Return);
        QCOMPARE(display->text(), QString("16"));
        QCOMPARE(window.findChild<QLabel *>("expressionLabel")->text(), QString("12.5 + 3.5 ="));
        QTest::keyClick(&window, Qt::Key_Escape);
        QCOMPARE(display->text(), QString("0"));
        QTest::keyClicks(&window, "5+9");
        QTest::keyClick(&window, Qt::Key_Delete);
        QTest::keyClicks(&window, "2");
        QTest::keyClick(&window, Qt::Key_Enter);
        QCOMPARE(display->text(), QString("7"));
    }

    void mouseAndKeyboardAreEquivalent()
    {
        MainWindow mouseWindow;
        MainWindow keyboardWindow;
        for (const QString &name : {"oneButton", "decimalButton", "fiveButton", "multiplyButton", "fourButton", "equalsButton"})
            QTest::mouseClick(mouseWindow.findChild<QPushButton *>(name), Qt::LeftButton);
        QTest::keyClicks(&keyboardWindow, "1.5*4=");
        QCOMPARE(mouseWindow.findChild<QLineEdit *>("displayEdit")->text(), keyboardWindow.findChild<QLineEdit *>("displayEdit")->text());
        QCOMPARE(mouseWindow.findChild<QLabel *>("expressionLabel")->text(), keyboardWindow.findChild<QLabel *>("expressionLabel")->text());
    }

    void childWidgetKeysAndModifiers()
    {
        MainWindow window;
        auto *display = window.findChild<QLineEdit *>("displayEdit");
        auto *button = window.findChild<QPushButton *>("fiveButton");
        QTest::keyClicks(button, "12+");
        QTest::keyClicks(display, "3");
        QTest::keyClick(button, Qt::Key_Return);
        QCOMPARE(display->text(), QString("15"));
        QTest::keyClick(&window, Qt::Key_C, Qt::ControlModifier);
        QCOMPARE(display->text(), QString("15"));
        QTest::keyClicks(&window, "abc");
        QCOMPARE(display->text(), QString("0"));
        QTest::keyClicks(&window, "xyz");
        QCOMPARE(display->text(), QString("0"));
    }

    void numpadInput()
    {
        MainWindow window;
        const auto send = [&window](int key, const QString &text) {
            QKeyEvent event(QEvent::KeyPress, key, Qt::KeypadModifier, text);
            QCoreApplication::sendEvent(&window, &event);
        };
        send(Qt::Key_2, "2");
        send(Qt::Key_Period, ".");
        send(Qt::Key_5, "5");
        send(Qt::Key_Asterisk, "*");
        send(Qt::Key_4, "4");
        send(Qt::Key_Enter, "\r");
        QCOMPARE(window.findChild<QLineEdit *>("displayEdit")->text(), QString("10"));
    }

    void errorAndLongNumberPresentation()
    {
        MainWindow window;
        auto *display = window.findChild<QLineEdit *>("displayEdit");
        QTest::keyClicks(&window, "8/0=");
        QCOMPARE(display->text(), QString::fromUtf8("不能除以零"));
        QVERIFY(display->property("error").toBool());
        saveQaSnapshot(window, "divide-by-zero.png");
        QTest::keyClicks(&window, "123456789012345");
        QVERIFY(!display->property("error").toBool());
        window.resize(window.minimumSize());
        window.show();
        QTest::qWait(30);
        QVERIFY(QFontMetrics(display->font()).horizontalAdvance(display->text()) <= display->contentsRect().width() - 24);
        saveQaSnapshot(window, "long-number.png");
    }

private:
    void saveQaSnapshot(MainWindow &window, const QString &name)
    {
        const QByteArray directory = qgetenv("CALCULATOR_QA_DIR");
        if (directory.isEmpty())
            return;
        const QString path = QString::fromLocal8Bit(directory);
        QVERIFY(QDir().mkpath(path));
        window.show();
        QTest::qWait(30);
        QVERIFY(window.grab().save(QDir(path).filePath(name)));
    }
};

QTEST_MAIN(CalculatorUiTest)
#include "test_calculator.moc"
