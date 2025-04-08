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
    virtual void updateInsulinLevel(float units);
    void lock();

    //for testing the occlusion
    void simulateOcclusion();
    void clearOcclusion();
    bool isOcclusionActive() const;
    void primeInfusionSet();

    //getter/setters
    virtual InsulinDelivery* getDeliverySystem() const;
    virtual AlertSystem* getAlerts() const;
    virtual DataLogger* getLogger() const;
    ControlIQ* getControlIQ() const;
    CGM* getGlucoseMonitor() const;
    void setActiveProfile(Profile* profile);


    float getBatteryLevel() const;
    virtual float getInsulinLevel() const;
    virtual bool isLocked() const;
    virtual Profile* getCurrentProfile() const;
    QList<Profile*>& getProfiles();

    void setBatteryLevel(float level);
    void setInsulinLevel(float level);

public slots:
    void pauseBasal();
    void resumeBasal();

signals:
    void batteryLevelChanged(float batteryLevel);

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
    bool occlusionActive = false;

};


#endif // PUMPSYSTEM_H
