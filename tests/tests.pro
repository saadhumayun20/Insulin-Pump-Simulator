TEMPLATE = app
CONFIG += console testcase
QT += core testlib

INCLUDEPATH += ../src

SOURCES += \
    test_profile.cpp \
    ../src/Profile.cpp

HEADERS += \
    ../src/Profile.h

