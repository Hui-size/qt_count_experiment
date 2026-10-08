#include "calculatorengine.h"
#include <QTest>

class CalculatorEngineTest : public QObject
{
    Q_OBJECT
private slots:
    void sequences_data()
    {
        QTest::addColumn<QString>("commands");
        QTest::addColumn<QString>("expected");
        QTest::newRow("digits") << "1 2 3" << "123";
        QTest::newRow("leading-zero") << "0 0 5" << "5";
        QTest::newRow("decimal-first") << "decimal 5" << "0.5";
        QTest::newRow("duplicate-decimal") << "1 decimal decimal 2" << "1.2";
        QTest::newRow("backspace-decimal") << "1 2 decimal 3 backspace backspace" << "12";
        QTest::newRow("empty-backspace") << "1 backspace backspace" << "0";
        QTest::newRow("clear") << "1 2 clear 3" << "3";
        QTest::newRow("clear-entry") << "1 2 clearEntry" << "0";
        QTest::newRow("sign") << "1 2 sign" << "-12";
        QTest::newRow("negative-backspace") << "1 sign backspace" << "0";
        QTest::newRow("zero-sign") << "0 sign" << "0";
        QTest::newRow("digit-limit") << "1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6" << "123456789012345";
        QTest::newRow("addition") << "1 2 + 3 equals" << "15";
        QTest::newRow("subtraction") << "2 - 5 equals" << "-3";
        QTest::newRow("multiplication") << "1 decimal 5 * 4 equals" << "6";
        QTest::newRow("division") << "9 / 4 equals" << "2.25";
        QTest::newRow("decimal-addition") << "0 decimal 1 + 0 decimal 2 equals" << "0.3";
        QTest::newRow("sequential-calculation") << "2 + 3 * 4 equals" << "20";
        QTest::newRow("replace-operator") << "8 + * 2 equals" << "16";
        QTest::newRow("equals-without-operator") << "5 equals" << "5";
        QTest::newRow("equals-without-second") << "5 + equals" << "5";
        QTest::newRow("divide-by-zero") << "8 / 0 equals" << "不能除以零";
        QTest::newRow("digit-after-result") << "2 + 3 equals 7" << "7";
        QTest::newRow("decimal-after-result") << "2 + 3 equals decimal 5" << "0.5";
        QTest::newRow("backspace-before-second") << "1 2 + backspace 3 equals" << "15";
        QTest::newRow("continue-from-result") << "2 + 3 equals * 4 equals" << "20";
        QTest::newRow("second-decimal") << "1 decimal 2 + 3 decimal decimal 4 equals" << "4.6";
        QTest::newRow("edit-second") << "1 2 + 3 4 backspace 5 equals" << "47";
        QTest::newRow("clear-pending") << "1 2 + 3 clear 4 * 2 equals" << "8";
        QTest::newRow("ce-keeps-operation") << "5 + 9 clearEntry 2 equals" << "7";
        QTest::newRow("ce-zero-second") << "5 + clearEntry equals" << "5";
        QTest::newRow("sign-pending") << "5 + sign 2 equals" << "7";
        QTest::newRow("negative-second") << "5 + 2 sign equals" << "3";
        QTest::newRow("backspace-result") << "1 2 + 3 equals backspace" << "15";
        QTest::newRow("recover-after-zero") << "8 / 0 equals 2 + 3 equals" << "5";
        QTest::newRow("decimal-recovery") << "8 / 0 equals decimal 5" << "0.5";
        QTest::newRow("zero-backspace") << "0 decimal 0 backspace backspace" << "0";
        QTest::newRow("repeated-equals") << "2 + 3 equals equals" << "5";
    }

    void sequences()
    {
        QFETCH(QString, commands);
        QFETCH(QString, expected);
        CalculatorEngine engine;
        for (const auto &command : commands.split(' ', Qt::SkipEmptyParts))
            engine.dispatch(command);
        QCOMPARE(engine.display(), expected);
    }

    void expressionState()
    {
        CalculatorEngine engine;
        engine.dispatch("1");
        engine.dispatch("2");
        engine.dispatch("+");
        QCOMPARE(engine.expression(), QString("12 +"));
        engine.dispatch("3");
        QCOMPARE(engine.expression(), QString("12 + 3"));
        engine.dispatch("equals");
        QCOMPARE(engine.expression(), QString("12 + 3 ="));
        QCOMPARE(engine.display(), QString("15"));
    }

    void stateTransitions()
    {
        CalculatorEngine engine;
        QCOMPARE(engine.state(), CalculatorEngine::State::FirstOperand);
        engine.dispatch("8");
        engine.dispatch("/");
        QCOMPARE(engine.state(), CalculatorEngine::State::OperatorPending);
        engine.dispatch("0");
        QCOMPARE(engine.state(), CalculatorEngine::State::SecondOperand);
        engine.dispatch("equals");
        QCOMPARE(engine.state(), CalculatorEngine::State::Error);
        QCOMPARE(engine.expression(), QString::fromUtf8("8 ÷ 0 ="));
        engine.dispatch("+");
        engine.dispatch("backspace");
        QVERIFY(engine.hasError());
        engine.dispatch("clearEntry");
        QCOMPARE(engine.state(), CalculatorEngine::State::FirstOperand);
        QCOMPARE(engine.display(), QString("0"));
        QVERIFY(engine.expression().isEmpty());
        for (const QString &command : {"2", "+", "3", "equals"})
            engine.dispatch(command);
        QCOMPARE(engine.state(), CalculatorEngine::State::Result);
        engine.dispatch("clearEntry");
        QVERIFY(engine.expression().isEmpty());
    }

    void overflowRecovery()
    {
        CalculatorEngine engine;
        const auto enterLargeNumber = [&engine] {
            for (int digit = 0; digit < 15; ++digit)
                engine.dispatch("9");
        };
        enterLargeNumber();
        for (int operation = 0; operation < 30 && !engine.hasError(); ++operation) {
            engine.dispatch("*");
            enterLargeNumber();
            engine.dispatch("equals");
        }
        QVERIFY(engine.hasError());
        QCOMPARE(engine.display(), QString::fromUtf8("结果超出范围"));
        engine.dispatch("clear");
        QCOMPARE(engine.display(), QString("0"));
        QVERIFY(engine.expression().isEmpty());
    }
};

QTEST_APPLESS_MAIN(CalculatorEngineTest)
#include "test_engine.moc"
