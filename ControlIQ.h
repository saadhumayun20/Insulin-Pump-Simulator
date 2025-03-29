#ifndef CONTROLIQ_H
#define CONTROLIQ_H

#include <QObject>
#include "CGM.h"

class ControlIQ : public QObject {
    Q_OBJECT

public:
    explicit ControlIQ(CGM* glucoseMonitor, QObject *parent = nullptr);
    CGM* getGlucoseMonitor() const;

public slots:
    void adjustBasalRate();
    void autoSuspend();

private:
    CGM* cgm;
    bool isEnabled;
    float targetGlucose;


};

#endif // CONTROLIQ_H
