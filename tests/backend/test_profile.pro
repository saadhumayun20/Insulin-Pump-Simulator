TEMPLATE = app
CONFIG += console testcase
QT += core testlib

INCLUDEPATH += ../src

SOURCES += \
    test_profile.cpp \
    ../../src/Profile.cpp

HEADERS += \
    ../../src/Profile.h

QMAKE_CLEAN += test_profile target_wrapper.sh *.o moc_*.cpp *.moc Makefile

