QT += core gui widgets
TEMPLATE = app
TARGET = qt_count_experiment
CONFIG += c++17 warn_on

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    calculatorengine.cpp

HEADERS += \
    mainwindow.h \
    calculatorengine.h

FORMS += mainwindow.ui
