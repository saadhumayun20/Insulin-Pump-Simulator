#include <QtTest>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include "../../src/mainwindow.h"

class GuiBolusTests : public QObject {
    Q_OBJECT
private:
    // Create a MainWindow instance for the tests.
    MainWindow mainWindow;
private slots:
    // Show the main window before tests run.
    void initTestCase() {
        mainWindow.show();
    }

    // Test that required bolus screen elements exist.
    void testBolusScreenElementsExist() {
        // Invoke the private navigateToBolus method.
        QMetaObject::invokeMethod(&mainWindow, "navigateToBolus", Qt::DirectConnection);
        // Find the elements by their object names.
        QLineEdit *txtCarbs = mainWindow.findChild<QLineEdit*>("txtCarbs");
        QLineEdit *txtGlucose = mainWindow.findChild<QLineEdit*>("txtGlucose");
        QLabel *lblBolusResult = mainWindow.findChild<QLabel*>("lblBolusResult");
        QVERIFY(txtCarbs != nullptr);
        QVERIFY(txtGlucose != nullptr);
        QVERIFY(lblBolusResult != nullptr);
    }

    // Test that the bolus calculation updates the result label.
    void testBolusCalculation() {
        QMetaObject::invokeMethod(&mainWindow, "navigateToBolus", Qt::DirectConnection);
        QLineEdit *txtCarbs = mainWindow.findChild<QLineEdit*>("txtCarbs");
        QLineEdit *txtGlucose = mainWindow.findChild<QLineEdit*>("txtGlucose");
        QLabel *lblBolusResult = mainWindow.findChild<QLabel*>("lblBolusResult");
        QVERIFY(txtCarbs && txtGlucose && lblBolusResult);
        txtCarbs->setText("30");
        txtGlucose->setText("180");
        QMetaObject::invokeMethod(&mainWindow, "handleBolusCalculation", Qt::DirectConnection);
        QVERIFY(!lblBolusResult->text().isEmpty());
    }

    // Test that the bolus screen is active (bolusScreen is the current widget).
    void testBolusDeliveryNavigation() {
        QMetaObject::invokeMethod(&mainWindow, "navigateToBolus", Qt::DirectConnection);
        QStackedWidget *stacked = mainWindow.findChild<QStackedWidget*>("stackedWidget");
        QVERIFY(stacked != nullptr);
        QWidget *currentScreen = stacked->currentWidget();
        // Assuming the bolus screen's objectName is "bolusScreen"
        QCOMPARE(currentScreen->objectName(), QString("bolusScreen"));
    }
};

QTEST_MAIN(GuiBolusTests)
#include "gui_bolus_tests.moc"

