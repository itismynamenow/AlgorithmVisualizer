include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt
QMAKE_CXXFLAGS += -save-temps

HEADERS += \
    aabb_test.h \
    quad_tree_test.h

SOURCES += \
        main.cpp
