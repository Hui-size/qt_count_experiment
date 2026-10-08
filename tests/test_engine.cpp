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
};

QTEST_APPLESS_MAIN(CalculatorEngineTest)
#include "test_engine.moc"
