#ifndef CONTROLIQ_H
#define CONTROLIQ_H

#include <QObject>
#include <QDateTime>
#include "CGM.h"
#include <QtMath>

class PumpSystem;

class ControlIQ : public QObject {
    Q_OBJECT

public:
    explicit ControlIQ(CGM* glucoseMonitor, PumpSystem* pump, QObject *parent = nullptr);
    CGM* getGlucoseMonitor() const;

public slots:
    void adjustBasalRate();
    void autoSuspend();
    void setLastCorrectionTime(const QDateTime& time);

private:
    void predictGlucose(float current, float trend, float& prediction);
    void deliverAutoCorrection(float predictedGlucose);

    CGM* cgm;
    PumpSystem* pump = nullptr;
    bool isEnabled = true;;
    QDateTime lastCorrectionTime;
    float lastBasalMultiplier = 1.0f;
    float targetGlucose;


};

#endif // CONTROLIQ_H
