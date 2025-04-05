#include "PumpSystem.h"

//AlertSystem* AlertSystem::m_instance = nullptr;

PumpSystem::PumpSystem(QObject *parent) :
    QObject(parent),
    currentProfile(nullptr),
    delivery(new InsulinDelivery(this)),
    alerts(AlertSystem::instance()),
    logger(new DataLogger(this)),
    controlIQ(new ControlIQ(new CGM(this), this, this)),
    cgm(controlIQ->getGlucoseMonitor()),
    batteryLevel(100.0f),
    insulinLevel(300.0f),
    locked(true)
{
    currentProfile = new Profile("Default", 1.0f, 15.0f, 50.0f, this);
    profiles.append(currentProfile);

    connect(cgm, &CGM::glucoseUpdated, controlIQ, &ControlIQ::adjustBasalRate);
}

void PumpSystem::powerOn(){
    if (batteryLevel <= 5.0f) {
        alerts->triggerLowBatteryWarning();
        return;
    }
    if (insulinLevel <= 10.0f) {
        alerts->triggerLowInsulinAlert();
        return;
    }

    controlIQ->adjustBasalRate();
}

void PumpSystem::powerOff(){
    delivery->stopBasal();
    logger->logDeliveryEvent(DeliveryEvent(DeliveryEvent::SUSPEND, 0.0f));
}

bool PumpSystem::unlock(int pin){
    locked = !(pin == 1234);
    return !locked;
}

void PumpSystem::chargeBattery() {
    batteryLevel = qMin(batteryLevel + 50.0f, 100.0f);
    emit batteryLevelChanged(batteryLevel);
}


void PumpSystem::updateInsulinLevel(float units){
    insulinLevel = qMax(insulinLevel - units, 0.0f);
}

void PumpSystem::lock() { locked = true; }

void PumpSystem::setActiveProfile(Profile *profile) {
    if(profile) {
        currentProfile = profile;
    }
}

//occlusion testing
void PumpSystem::simulateOcclusion() { occlusionActive = true; }
void PumpSystem::clearOcclusion() { occlusionActive = false; }
bool PumpSystem::isOcclusionActive() const {return occlusionActive; }

//getter
float PumpSystem::getBatteryLevel() const { return batteryLevel; }
float PumpSystem::getInsulinLevel() const { return insulinLevel; }
bool PumpSystem::isLocked() const { return locked; }
Profile* PumpSystem::getCurrentProfile() const { return currentProfile; }
InsulinDelivery* PumpSystem::getDeliverySystem() const { return delivery; }
AlertSystem* PumpSystem::getAlerts() const { return alerts; }
DataLogger* PumpSystem::getLogger() const { return logger; }
ControlIQ* PumpSystem::getControlIQ() const { return controlIQ; }
CGM* PumpSystem::getGlucoseMonitor() const { return cgm; }
QList<Profile*>& PumpSystem::getProfiles() { return profiles; }

void PumpSystem::setBatteryLevel(float level) {
    batteryLevel = qBound(0.0f, level, 100.0f);
}

void PumpSystem::setInsulinLevel(float level) {
    insulinLevel = qMax(0.0f, level);
}
