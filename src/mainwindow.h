#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

#include "PumpSystem.h"
#include "AlertSystem.h"
#include "CGM.h"
#include "DeliveryEvent.h"

//  chart display
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation handlers
    void navigateToHome();
    void navigateToBolus();
    void navigateToProfiles();
    void navigateToHistory();
    void navigateToPrime();
    void navigateToSettings();

    // Functionality handlers
    void handleBolusCalculation();
    void deliverBolus();
    void addProfile();
    void editProfile();
    void deleteProfile();
    void handleUnlock();
    void setActiveProfile();

    // UI update handlers
    void updateUI();
    void updateGlucoseChart(float glucose);
    void handleAlert(const QString& message);

private:
    Ui::MainWindow *ui;
    PumpSystem *pumpSystem;
    QTimer *updateTimer;
    QTimer *historyUpdateTimer;
    QLineSeries *glucoseSeries;

    QChart *glucoseChart;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;

    void setupConnections();
    void initializeUI();
    void setupCharts();
    void updateDeliveryHistory();
    void checkAndShowLockScreen();
};

#endif // MAINWINDOW_H
