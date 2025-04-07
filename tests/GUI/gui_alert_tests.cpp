#include <QtTest>
#include <QLabel>
#include <QPushButton>
#include "../../src/mainwindow.h"

class GuiAlertTests : public QObject {
    Q_OBJECT

private:
    MainWindow mainWindow;

private slots:
    void initTestCase() {
        mainWindow.show();
    }

    void testLowBatteryAlertAppears() {
        QMetaObject::invokeMethod(&mainWindow, "handleAlert", Qt::DirectConnection,
                                  Q_ARG(QString, "Low Battery"));
        QLabel *lblAlert = mainWindow.findChild<QLabel*>("lblAlert");
        QVERIFY(lblAlert != nullptr);
        QCOMPARE(lblAlert->text(), QString("Low Battery"));
    }

    void testLowInsulinAlertAppears() {
        QMetaObject::invokeMethod(&mainWindow, "handleAlert", Qt::DirectConnection,
                                  Q_ARG(QString, "Low Insulin"));
        QLabel *lblAlert = mainWindow.findChild<QLabel*>("lblAlert");
        QVERIFY(lblAlert != nullptr);
        QCOMPARE(lblAlert->text(), QString("Low Insulin"));
    }

    void testOcclusionAlertAppears() {
        QMetaObject::invokeMethod(&mainWindow, "handleAlert", Qt::DirectConnection,
                                  Q_ARG(QString, "Occlusion Detected"));
        QLabel *lblAlert = mainWindow.findChild<QLabel*>("lblAlert");
        QVERIFY(lblAlert != nullptr);
        QCOMPARE(lblAlert->text(), QString("Occlusion Detected"));
    }

    void testAlertClears() {
        QMetaObject::invokeMethod(&mainWindow, "handleAlert", Qt::DirectConnection,
                                  Q_ARG(QString, "clear"));
        QLabel *lblAlert = mainWindow.findChild<QLabel*>("lblAlert");
        QVERIFY(lblAlert != nullptr);
        QVERIFY(lblAlert->text().isEmpty());
    }
};

QTEST_MAIN(GuiAlertTests)
#include "gui_alert_tests.moc"

