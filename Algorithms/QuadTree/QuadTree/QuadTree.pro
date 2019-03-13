QMAKE_CXXFLAGS *= -std=c++11
#QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS_DEBUG *= -static-libgcc -static-libstdc++ -g -O0
QMAKE_LFLAGS_RELEASE -= -s

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuadTreeCPP
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
    main_window.h \
    quad_tree.h \
    quad_tree_benchmark.h \
    quad_tree_fast.h \
    quad_tree_moderate.h \
    quad_tree_slow.h \
    quad_tree_widget.h

SOURCES += \
    main.cpp
