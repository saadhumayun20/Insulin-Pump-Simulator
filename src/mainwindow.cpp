#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pumpSystem = new PumpSystem(this);
    pumpSystem->powerOn();

    initializeUI();
    setupConnections();

    glucoseChart = new QChart();
    glucoseSeries = new QLineSeries();
    axisX = new QDateTimeAxis();
    axisY = new QValueAxis();

    setupCharts();
    ui->glucoseChart->setChart(glucoseChart);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateUI);
    updateTimer->start(1000);


    // Simulate glucose updates
    QTimer *glucoseTimer = new QTimer(this);
    connect(glucoseTimer, &QTimer::timeout, [this]() {
        static float glucose = 120.0f;
        glucose += (rand() % 11 - 5);
        pumpSystem->getGlucoseMonitor()->updateGlucose(glucose);
    });
    glucoseTimer->start(2500); // Update every 2.5 seconds

    historyUpdateTimer = new QTimer(this);
    connect(historyUpdateTimer, &QTimer::timeout, this, &MainWindow::updateDeliveryHistory);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, [=](int index) {
        if (ui->stackedWidget->widget(index) == ui->historyScreen) {
            historyUpdateTimer->start(2500); // Update every 2.5 seconds
        } else {
            historyUpdateTimer->stop();
        }
    });

    checkAndShowLockScreen();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initializeUI() {
    // initial screen to home
    qApp->setStyleSheet(
                "QWidget {"
                "   font-family: 'Segoe UI';"
                "   font-size: 12pt;"
                "   background-color: #f9f9f9;" // Light gray background
                "}"
                ""
                "QPushButton {"
                "   background-color: #0078d4;" // Blue buttons
                "   color: white;"
                "   border-radius: 5px;"
                "   padding: 10px;"
                "}"
                ""
                "QPushButton:hover {"
                "   background-color: #005a9e;"
                "}"
                ""
                "QPushButton#btnLock {"
                "   background-color: #d13438;"
                "}"
                ""
                "QPushButton#btnLock:hover {"
                "   background-color: #a4262c;"
                "}"
                ""
                "QLabel#lblBattery, QLabel#lblInsulin, QLabel#lblIOB {"
                "   background-color: #ffffff;"
                "   border: 1px solid #cccccc;"
                "   padding: 5px;"
                "   border-radius: 5px;"
                "   font-weight: bold;"
                "}"
                ""
                "QTableWidget {"
                "   background-color: white;"
                "   alternate-background-color: #f2f2f2;"
                "   gridline-color: #cccccc;"
                "}"
                ""
                "QHeaderView::section {"
                "   background-color: #0078d4;"
                "   color: white;"
                "   padding: 5px;"
                "}"
                );
    ui->glucoseChart->setStyleSheet(
                "QChartView {"
                "   border: 1px solid #cccccc;"
                "   border-radius: 5px;"
                "} "
                );

    ui->stackedWidget->setCurrentWidget(ui->homeScreen);

    ui->txtCarbs->setPlaceholderText("Enter carbs (g)");
    ui->txtGlucose->setPlaceholderText("Enter glucose (mg/dL)");

    // Initialize table
    ui->deliveryHistory->setColumnCount(3);
    ui->deliveryHistory->setHorizontalHeaderLabels({"Time", "Type", "Units"});
    ui->deliveryHistory->horizontalHeader()->setStretchLastSection(true);

    // update initial values
    ui->lblBattery->setText(QString("Battery: %1%").arg(pumpSystem->getBatteryLevel()));
    ui->lblInsulin->setText(QString("Insulin: %1U").arg(pumpSystem->getInsulinLevel()));
    ui->lblIOB->setText(QString("IOB: 0.0U"));
}

void MainWindow::setupConnections() {
    // Navigation connections
    connect(ui->btnBolus, &QPushButton::clicked, this, &MainWindow::navigateToBolus);
    connect(ui->btnProfiles, &QPushButton::clicked, this, &MainWindow::navigateToProfiles);
    connect(ui->btnHistory, &QPushButton::clicked, this, &MainWindow::navigateToHistory);
    connect(ui->btnMaintenance, &QPushButton::clicked, this, &MainWindow::navigateToPrime);
    connect(ui->btnSettings, &QPushButton::clicked, this, &MainWindow::navigateToSettings);
    connect(ui->btnLock, &QPushButton::clicked, [=](){pumpSystem->lock();ui->stackedWidget->setCurrentWidget(ui->lockScreen);});

    // Bolus screen connections
    connect(ui->btnCalculate, &QPushButton::clicked, this, &MainWindow::handleBolusCalculation);
    connect(ui->btnDeliver, &QPushButton::clicked, this, &MainWindow::deliverBolus);
    connect(ui->btnBack_3, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentWidget(ui->homeScreen);});


    connect(ui->btnQuickBolus, &QPushButton::clicked, [=](){
        pumpSystem->getDeliverySystem()->quickBolus();
    });

    // Profile screen connections
    connect(ui->btnAddProfile, &QPushButton::clicked, this, &MainWindow::addProfile);
    connect(ui->btnSetActive, &QPushButton::clicked, this, &MainWindow::setActiveProfile);
    connect(ui->btnEditProfile, &QPushButton::clicked, this, &MainWindow::editProfile);
    connect(ui->btnDeleteProfile, &QPushButton::clicked, this, &MainWindow::deleteProfile);
    connect(ui->btnBack, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentWidget(ui->homeScreen);});
    connect(ui->btnBack_2, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentWidget(ui->homeScreen);});


    // Lock screen connection
    connect(ui->btnUnlock, &QPushButton::clicked, this, &MainWindow::handleUnlock);

    // Alert connections
    connect(pumpSystem->getAlerts(), &AlertSystem::alertTriggered,
            this, &MainWindow::handleAlert);

    // CGM connections
    connect(pumpSystem->getGlucoseMonitor(), &CGM::glucoseUpdated,
            this, &MainWindow::updateGlucoseChart);

    // Test buttons
    connect(ui->btnTestLowBattery, &QPushButton::clicked, [=](){
        pumpSystem->setBatteryLevel(3.0f); // Simulate critical battery
        pumpSystem->getAlerts()->triggerLowBatteryWarning();
    });

    connect(pumpSystem, &PumpSystem::batteryLevelChanged, [=](float level){
        ui->lblBattery->setText(QString("%1%").arg(level));
    });


    connect(ui->btnTestLowInsulin, &QPushButton::clicked, [=](){
        pumpSystem->setInsulinLevel(5.0f); // Simulate low insulin
        pumpSystem->getAlerts()->triggerLowInsulinAlert();
    });

    connect(ui->btnTestOcclusion, &QPushButton::clicked, [=](){
        pumpSystem->simulateOcclusion();
        pumpSystem->getAlerts()->triggerOcclusionAlarm();
    });

    //BASAL BUTTONS
    connect(ui->btnPauseBasal, &QPushButton::clicked, [=](){
        pumpSystem->pauseBasal();
        ui->btnPauseBasal->setEnabled(false);
        ui->btnResumeBasal->setEnabled(true);
    });

    connect(ui->btnResumeBasal, &QPushButton::clicked, [=](){
        pumpSystem->resumeBasal();
        ui->btnPauseBasal->setEnabled(true);
        ui->btnResumeBasal->setEnabled(false);
    });

    connect(ui->btnPrime, &QPushButton::clicked, [=](){
        pumpSystem->primeInfusionSet();
        updateUI();
    });

    connect(ui->btnBack_4, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentWidget(ui->homeScreen);});

    //buttons for settings
    connect(ui->btnPairCGM, &QPushButton::clicked, [=](){
        pumpSystem->getGlucoseMonitor()->pairDevice();
    });

    connect(pumpSystem->getGlucoseMonitor(), &CGM::pairedStatusChanged, [=](bool paired){
        ui->lblCGMStatus->setText(paired ? "Status: Paired" : "Status: Not Paired");
    });

    connect(ui->btnBack_5, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentWidget(ui->homeScreen);});
}

void MainWindow::setupCharts() {

    glucoseSeries->setName("Glucose (mg/dL)");
    glucoseChart->addSeries(glucoseSeries);

    // Axes
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    axisX->setTickCount(10);
    glucoseChart->addAxis(axisX, Qt::AlignBottom);
    glucoseSeries->attachAxis(axisX);

    axisY->setRange(40, 400);
    axisY->setTitleText("mg/dL");
    glucoseChart->addAxis(axisY, Qt::AlignLeft);
    glucoseSeries->attachAxis(axisY);

    //  rendering settings
    ui->glucoseChart->setMinimumSize(800, 300);
    ui->verticalLayout_History->setStretch(0, 3);
    ui->verticalLayout_History->setStretch(1, 1);
    ui->glucoseChart->setRenderHint(QPainter::Antialiasing, true);
    glucoseChart->setAnimationOptions(QChart::NoAnimation);
}


void MainWindow::navigateToHome() {
    ui->stackedWidget->setCurrentWidget(ui->homeScreen);
}

void MainWindow::navigateToBolus() {
    ui->stackedWidget->setCurrentWidget(ui->bolusScreen);
    ui->txtCarbs->clear();
    ui->txtGlucose->setText(QString::number(pumpSystem->getGlucoseMonitor()->readGlucose()));
}

void MainWindow::navigateToProfiles() {
    ui->stackedWidget->setCurrentWidget(ui->profileScreen);

    ui->lstProfiles->clear();
    foreach(Profile* profile, pumpSystem->getProfiles()) {
        ui->lstProfiles->addItem(profile->getName());
    }

    Profile* activeProfile = pumpSystem->getCurrentProfile();
    if (activeProfile) {
        for (int i = 0; i < ui->lstProfiles->count(); i++) {
            if (ui->lstProfiles->item(i)->text() == activeProfile->getName()) {
                ui->lstProfiles->setCurrentRow(i);
                break;
            }
        }
    }
}

void MainWindow::navigateToHistory() {
    ui->stackedWidget->setCurrentWidget(ui->historyScreen);
    ui->glucoseChart->repaint();
    updateDeliveryHistory();
}

void MainWindow::navigateToPrime() {
    ui->stackedWidget->setCurrentWidget(ui->maintenanceScreen);
}

void MainWindow::navigateToSettings() {
    ui->stackedWidget->setCurrentWidget(ui->settingsPage);
}

void MainWindow::handleBolusCalculation() {
    float carbs = ui->txtCarbs->text().toFloat();
    float glucose = ui->txtGlucose->text().toFloat();

    float units = pumpSystem->getDeliverySystem()->calculateBolus(glucose, carbs);
    ui->lblBolusResult->setText(QString("Suggested: %1U").arg(units));
}

void MainWindow::deliverBolus() {
    QString resultText = ui->lblBolusResult->text();
    if (resultText.isEmpty()) return;

    float units = resultText.split(": ")[1].replace("U", "").toFloat();
    if (units <= 0) return;

    pumpSystem->getDeliverySystem()->deliverBolus(units);
    navigateToHome();
}

void MainWindow::addProfile() {
    bool ok;
    QString name = QInputDialog::getText(this, "New Profile", "Profile Name:",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    float basalRate = QInputDialog::getDouble(this, "Basal Rate",
                                              "Enter basal rate (U/hr):", 1.0, 0.0, 10.0, 2, &ok);
    if (!ok) return;

    float carbRatio = QInputDialog::getDouble(this, "Carb Ratio",
                                              "Enter carb ratio (g/U):", 15.0, 1.0, 150.0, 1, &ok);
    if (!ok) return;

    float correctionFactor = QInputDialog::getDouble(this, "Correction Factor",
                                                     "Enter correction factor (mg/dL/U):", 50.0, 10.0, 200.0, 1, &ok);
    if (!ok) return;

    Profile* newProfile = new Profile(name, basalRate, carbRatio, correctionFactor, pumpSystem);
    pumpSystem->getProfiles().append(newProfile);

    navigateToProfiles(); // Refresh the list
}

void MainWindow::editProfile() {
    QListWidgetItem* item = ui->lstProfiles->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Error", "Please select a profile to edit");
        return;
    }

    QString name = item->text();
    Profile* profile = nullptr;

    // Find the profile
    foreach(Profile* p, pumpSystem->getProfiles()) {
        if (p->getName() == name) {
            profile = p;
            break;
        }
    }

    if (!profile) return;

    bool ok;
    float basalRate = QInputDialog::getDouble(this, "Basal Rate",
                                              "Enter basal rate (U/hr):",
                                              profile->getBasalRate(), 0.0, 10.0, 2, &ok);
    if (!ok) return;

    float carbRatio = QInputDialog::getDouble(this, "Carb Ratio",
                                              "Enter carb ratio (g/U):",
                                              profile->getCarbRatio(), 1.0, 150.0, 1, &ok);
    if (!ok) return;

    float correctionFactor = QInputDialog::getDouble(this, "Correction Factor",
                                                     "Enter correction factor (mg/dL/U):",
                                                     profile->getCorrectionFactor(), 10.0, 200.0, 1, &ok);
    if (!ok) return;

    profile->editProfile(basalRate, carbRatio, correctionFactor);
}

void MainWindow::deleteProfile() {
    QListWidgetItem* item = ui->lstProfiles->currentItem();

    if (!item) {
        QMessageBox::warning(this, "Error", "Please select a profile to delete");
        return;
    }

    QString name = item->text();
    int index = -1;

    for (int i = 0; i < pumpSystem->getProfiles().size(); i++) {
        if (pumpSystem->getProfiles()[i]->getName() == name) {
            index = i;
            break;
        }
    }

    if (index >= 0 && index < pumpSystem->getProfiles().size()) {
        pumpSystem->getProfiles().removeAt(index);
        delete ui->lstProfiles->takeItem(ui->lstProfiles->row(item));
    } else {
        QMessageBox::warning(this, "Error", "Profile not found in system");
    }
}


void MainWindow::handleUnlock() {
    int pin = ui->txtPIN->text().toInt();
    bool unlocked = pumpSystem->unlock(pin);

    if (unlocked) {
        ui->stackedWidget->setCurrentWidget(ui->homeScreen);
        ui->txtPIN->clear();
        statusBar()->showMessage("Pump unlocked", 3000);
    } else {
        QMessageBox::warning(this, "Error", "Invalid PIN");
        ui->txtPIN->clear();
        ui->txtPIN->setFocus();
    }
}

void MainWindow::setActiveProfile() {
    QListWidgetItem* selectedItem = ui->lstProfiles->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Error", "Please select a profile to set as active");
        return;
    }

    QString name = selectedItem->text();
    Profile* profile = nullptr;

    // Find  profile
    foreach(Profile* p, pumpSystem->getProfiles()) {
        if (p->getName() == name) {
            profile = p;
            break;
        }
    }

    if (profile) {
        pumpSystem->setActiveProfile(profile);
        QMessageBox::information(this, "Success", "Profile set as active");
    } else {
        QMessageBox::warning(this, "Error", "Failed to set profile as active");
    }
}


// UI update handlers
void MainWindow::updateUI() {

    ui->lblBattery->setText(QString("Battery: %1%").arg(pumpSystem->getBatteryLevel()));
    ui->lblInsulin->setText(QString("Insulin: %1U").arg(pumpSystem->getInsulinLevel()));
    ui->lblIOB->setText(QString("IOB: %1U").arg(pumpSystem->getDeliverySystem()->getActiveIOB()));

    if (pumpSystem->getAlerts()->isOcclusionAlarmActive()) {
        ui->statusbar->showMessage("ALARM: Occlusion detected", 0);
    } else if (pumpSystem->getAlerts()->isLowInsulinAlarmActive()) {
        ui->statusbar->showMessage("ALERT: Low insulin", 0);
    } else if (pumpSystem->getAlerts()->isLowBatteryAlarmActive()) {
        ui->statusbar->showMessage("ALERT: Low battery", 0);
    }
}

void MainWindow::updateGlucoseChart(float glucose) {
    // Append new data to existing series
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    glucoseSeries->append(timestamp, glucose);

    // Adjust x-axis range to show last 10 minutes
    qint64 tenMinutesAgo = timestamp - 10 * 60 * 1000;
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(tenMinutesAgo),
                    QDateTime::fromMSecsSinceEpoch(timestamp));
}


void MainWindow::updateDeliveryHistory() {
    // Fetch the latest delivery event from the logger
    const QList<DeliveryEvent>& history = pumpSystem->getLogger()->getDeliveryHistory();
    if (history.isEmpty()) return; // No events to update

    const DeliveryEvent& event = history.last();

    // Add event to the table
    int row = ui->deliveryHistory->rowCount();
    ui->deliveryHistory->insertRow(row);

    QString formattedTime = event.getTimeStamp().toString("hh:mm:ss");
    QTableWidgetItem* timeItem = new QTableWidgetItem(formattedTime);
    QTableWidgetItem* typeItem = new QTableWidgetItem(
                event.getType() == DeliveryEvent::BASAL ? "Basal" :
                                                          event.getType() == DeliveryEvent::BOLUS ? "Bolus" :
                                                                                                    event.getType() == DeliveryEvent::SUSPEND ? "Suspend" :
                                                                                                                                                "Extended Bolus"
                                                                                                                                                );
    QTableWidgetItem* unitsItem = new QTableWidgetItem(QString("%1U").arg(event.getUnits()));

    ui->deliveryHistory->setItem(row, 0, timeItem);
    ui->deliveryHistory->setItem(row, 1, typeItem);
    ui->deliveryHistory->setItem(row, 2, unitsItem);

    // Add data to chart using the same timestamp
    qint64 timestamp = event.getTimeStamp().toMSecsSinceEpoch();
    glucoseSeries->append(timestamp, pumpSystem->getGlucoseMonitor()->readGlucose());
}


void MainWindow::handleAlert(const QString& message) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(message);

    if (message.contains("Battery")) {
        //ui->btnPlugIn->setVisible(true);
        QPushButton* plugInButton = msgBox.addButton("Plug In", QMessageBox::ActionRole);
        connect(plugInButton, &QPushButton::clicked, [=](){
            pumpSystem->chargeBattery();
            pumpSystem->getAlerts()->clearAlerts();
            //ui->btnPlugIn->setVisible(false);
            updateUI();
        });
    }
    else if (message.contains("Insulin")) {
        QPushButton* refillButton = msgBox.addButton("Refill", QMessageBox::ActionRole);
        connect(refillButton, &QPushButton::clicked, [=](){
            pumpSystem->setInsulinLevel(300.0f);
            pumpSystem->getAlerts()->clearAlerts();
            updateUI();
        });
    }
    else if (message.contains("Occlusion")) {
        QPushButton* clearButton = msgBox.addButton("Clear Occlusion", QMessageBox::ActionRole);
        connect(clearButton, &QPushButton::clicked, [=](){
            pumpSystem->clearOcclusion();
            pumpSystem->getAlerts()->clearAlerts();
            updateUI();
        });
    }

    msgBox.exec();
}


void MainWindow::checkAndShowLockScreen() {
    if (pumpSystem->isLocked()) {
        ui->stackedWidget->setCurrentWidget(ui->lockScreen);
        ui->txtPIN->setFocus();
    }
}
