include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

HEADERS += \
    kd_tree_test.h \
    aabb_test.h

SOURCES += \
        main.cpp
