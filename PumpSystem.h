#ifndef PUMPSYSTEM_H
#define PUMPSYSTEM_H

#include <QObject>
#include <QTimer>
#include "Profile.h"
#include "InsulinDelivery.h"
#include "AlertSystem.h"
#include "DataLogger.h"
#include "ControlIQ.h"
#include "CGM.h"

class PumpSystem : public QObject {
    Q_OBJECT

public:
    explicit PumpSystem(QObject *parent = nullptr);

    void powerOn();
    void powerOff();
    bool unlock(int pin);
    void chargeBattery();
    void updateInsulinLevel(float units);
    void lock();


    InsulinDelivery* getDeliverySystem() const;
    AlertSystem* getAlerts() const;
    DataLogger* getLogger() const;
    ControlIQ* getControlIQ() const;
    CGM* getGlucoseMonitor() const;

    float getBatteryLevel() const;
    float getInsulinLevel() const;
    bool isLocked() const;
    Profile* getCurrentProfile() const;
    QList<Profile*>& getProfiles();

private:
    Profile* currentProfile;
    QList<Profile*> profiles;
    InsulinDelivery* delivery;
    AlertSystem* alerts;
    DataLogger* logger;
    ControlIQ* controlIQ;
    CGM* cgm;
    float batteryLevel;
    float insulinLevel;
    bool locked;

};


#endif // PUMPSYSTEM_H
