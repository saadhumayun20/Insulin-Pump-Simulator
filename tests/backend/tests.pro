TEMPLATE = app
CONFIG += console testcase
QT += core testlib

INCLUDEPATH += ../src

SOURCES += \
    test_profile.cpp \
    test_cgm.cpp \
    ../src/Profile.cpp \
    ../src/CGM.cpp

HEADERS += \
    ../src/Profile.h \
    ../src/CGM.h

QMAKE_CLEAN += tests target_wrapper.sh *.o moc_*.cpp *.moc Makefile

