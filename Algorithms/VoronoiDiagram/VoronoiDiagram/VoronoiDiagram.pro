TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -save-temps

SOURCES += \
        main.cpp

HEADERS += \
    delaunator.h \
    voronoi_diagram.h
