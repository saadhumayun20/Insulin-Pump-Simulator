QT       += core gui charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/AlertSystem.cpp \
    src/CGM.cpp \
    src/Cartridge.cpp \
    src/ControlIQ.cpp \
    src/DataLogger.cpp \
    src/DeliveryEvent.cpp \
    src/InfusionSet.cpp \
    src/InsulinDelivery.cpp \
    src/Profile.cpp \
    src/PumpSystem.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/AlertSystem.h \
    src/CGM.h \
    src/Cartridge.h \
    src/ControlIQ.h \
    src/DataLogger.h \
    src/DeliveryEvent.h \
    src/InfusionSet.h \
    src/InsulinDelivery.h \
    src/Profile.h \
    src/PumpSystem.h \
    src/mainwindow.h

FORMS += \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
    src/resources.qrc

DISTFILES +=
