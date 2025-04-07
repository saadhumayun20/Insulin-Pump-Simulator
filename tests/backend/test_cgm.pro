TEMPLATE = app
CONFIG += console testcase
QT += core testlib

INCLUDEPATH += ../src

SOURCES += \
    test_cgm.cpp \
    ../src/CGM.cpp

HEADERS += \
    ../src/CGM.h

QMAKE_CLEAN += test_cgm target_wrapper.sh *.o moc_*.cpp *.moc Makefile

