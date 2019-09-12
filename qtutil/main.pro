QT -= gui
QT += serialport

CONFIG += console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++2a
CONFIG += release
CONFIG += static

isEmpty(PROJECT) {
        error(Not set project type)
}

TARGET = $$PROJECT

win32-g++ {
        QMAKE_LFLAGS += "-static -static-libgcc -static-libstdc++ -lstdc++"
        DEFINES += STATIC
}

INCLUDEPATH += \
    $$PWD/library

include(./$$PROJECT/logic.pro)

target.path += $$PWD/release/

INSTALLS += target
