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

# 在 Qt Creator 中使用 Qt 6.10.3 MinGW 64-bit Kit。
# 构建目录由 Qt Creator 单独设置，避免生成文件混入源代码。
