TEMPLATE = app

CONFIG += c++14
QMAKE_CXXFLAGS_CXX11    = -std=c++1y

QT += charts
QT += qml quick widgets
QT += serialport
QT += network

SOURCES += main.cpp \
    tcpserver.cpp \
    mainwindow.cpp \
    simulatorstate.cpp \
    simulatorstatehistory.cpp \
    diagnostics.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =


HEADERS += \
    tcpserver.h \
    mainwindow.h \
    simulatorstate.h \
    simulatorstatehistory.h \
    diagnostics.h
