#include "calculatorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Qt Calculator");
    app.setApplicationVersion("1.0");
    CalculatorWindow window;
    window.show();
    return app.exec();
}
