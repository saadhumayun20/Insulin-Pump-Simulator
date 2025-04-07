QT += testlib widgets charts

SOURCES += gui_bolus_tests.cpp \
           ../../src/mainwindow.cpp \
           ../../src/PumpSystem.cpp \
           ../../src/Profile.cpp \
           ../../src/CGM.cpp \
           ../../src/InsulinDelivery.cpp \
           ../../src/AlertSystem.cpp \
           ../../src/DataLogger.cpp \
           ../../src/DeliveryEvent.cpp \
           ../../src/ControlIQ.cpp  # ← this line is new!

HEADERS += ../../src/mainwindow.h \
           ../../src/PumpSystem.h \
           ../../src/Profile.h \
           ../../src/CGM.h \
           ../../src/InsulinDelivery.h \
           ../../src/AlertSystem.h \
           ../../src/DataLogger.h \
           ../../src/DeliveryEvent.h \
           ../../src/ControlIQ.h  # ← also add the header

FORMS += ../../src/mainwindow.ui

TARGET = gui_bolus_test
CONFIG += console
QMAKE_CLEAN += gui_bolus_test

