QT += testlib widgets charts

SOURCES += gui_alert_tests.cpp \
           ../../src/mainwindow.cpp \
           ../../src/PumpSystem.cpp \
           ../../src/Profile.cpp \
           ../../src/CGM.cpp \
           ../../src/InsulinDelivery.cpp \
           ../../src/AlertSystem.cpp \
           ../../src/DataLogger.cpp \
           ../../src/DeliveryEvent.cpp \
	   ../../src/ControlIQ.cpp
HEADERS += ../../src/mainwindow.h \
           ../../src/PumpSystem.h \
           ../../src/Profile.h \
           ../../src/CGM.h \
           ../../src/InsulinDelivery.h \
           ../../src/AlertSystem.h \
           ../../src/DataLogger.h \
           ../../src/DeliveryEvent.h \
	   ../../src/ControlIQ.h
FORMS += ../../src/mainwindow.ui

TARGET = gui_alert_test
CONFIG += console
QMAKE_CLEAN += gui_alert_test

