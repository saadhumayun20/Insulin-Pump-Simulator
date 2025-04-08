QT += core testlib

CONFIG += console c++11
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += test_insulindelivery.cpp \
    ../../src/InsulinDelivery.cpp \
    ../../src/PumpSystem.cpp \
    ../../src/Profile.cpp \
    ../../src/DataLogger.cpp \
    ../../src/DeliveryEvent.cpp \
    ../../src/AlertSystem.cpp \
    ../../src/CGM.cpp \
    ../../src/ControlIQ.cpp

HEADERS += \
    ../../src/InsulinDelivery.h \
    ../../src/PumpSystem.h \
    ../../src/Profile.h \
    ../../src/DataLogger.h \
    ../../src/DeliveryEvent.h \
    ../../src/AlertSystem.h \
    ../../src/CGM.h \
    ../../src/ControlIQ.h


QMAKE_CLEAN += test_insulindelivery

