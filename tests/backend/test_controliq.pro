TEMPLATE = app
CONFIG += console testcase
QT += core testlib

INCLUDEPATH += ../src

SOURCES += \
    test_controliq.cpp \
    ../../src/ControlIQ.cpp \
    ../../src/CGM.cpp \
    ../../src/Profile.cpp \
    ../../src/PumpSystem.cpp \
    ../../src/InsulinDelivery.cpp \
    ../../src/DeliveryEvent.cpp \
    ../../src/DataLogger.cpp \
    ../../src/AlertSystem.cpp

HEADERS += \
    ../../src/ControlIQ.h \
    ../../src/CGM.h \
    ../../src/Profile.h \
    ../../src/PumpSystem.h \
    ../../src/InsulinDelivery.h \
    ../../src/DeliveryEvent.h \
    ../../src/DataLogger.h \
    ../../src/AlertSystem.h

QMAKE_CLEAN += test_controliq *.o moc_*.cpp *.moc Makefile

