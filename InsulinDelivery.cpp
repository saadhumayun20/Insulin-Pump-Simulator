#include "InsulinDelivery.h"
#include "PumpSystem.h"

InsulinDelivery::InsulinDelivery(PumpSystem* parentSystem)
    : QObject(parentSystem),
      parentSystem(parentSystem),
      currentBasalRate(0.0f),
      ioBUnits(0.0f)
{
    // IOB Decay Timer (5% per hour exponential decay)
    QTimer* decayTimer = new QTimer(this);
    connect(decayTimer, &QTimer::timeout, [this](){
        const float decayRate = 0.05f / 60;
        ioBUnits *= qExp(-decayRate);
        ioBUnits = qMax(ioBUnits, 0.0f);
    });
    decayTimer->start(60000); // Every minute

    QTimer* basalTimer = new QTimer(this);
    connect(basalTimer, &QTimer::timeout, [this](){
        if(this->currentBasalRate > 0 && this->parentSystem->getInsulinLevel() > 0) {
            const float perSecondDelivery = this->currentBasalRate / 3600.0f;
            this->parentSystem->updateInsulinLevel(perSecondDelivery);
            this->ioBUnits += perSecondDelivery;
        }
    });
    basalTimer->start(1000); // Every second
}

void InsulinDelivery::startBasal()
{
    if(parentSystem->isLocked()) return;

    currentBasalRate = parentSystem->getCurrentProfile()->getBasalRate();
    parentSystem->getLogger()->logDeliveryEvent(
        DeliveryEvent(DeliveryEvent::BASAL, currentBasalRate)
    );
}

void InsulinDelivery::stopBasal()
{
    currentBasalRate = 0.0f;
    parentSystem->getLogger()->logDeliveryEvent(
        DeliveryEvent(DeliveryEvent::SUSPEND, 0.0f)
    );
}

void InsulinDelivery::deliverBolus(float units)
{
    if(units <= 0.0f || units > 25.0f) {
        parentSystem->getAlerts()->triggerOcclusionAlarm();
        return;
    }

    if(units > parentSystem->getInsulinLevel()) {
        parentSystem->getAlerts()->triggerLowInsulinAlert();
        return;
    }

    float delivered = 0.0f;
    QTimer* bolusTimer = new QTimer(this);
    QObject::connect(bolusTimer, &QTimer::destroyed, [this, units](){
        parentSystem->getLogger()->logDeliveryEvent(
            DeliveryEvent(DeliveryEvent::BOLUS, units)
        );
    });

    QObject::connect(bolusTimer, &QTimer::timeout, [=]() mutable {
        const float increment = units / 8.0f;

        delivered += increment;
        ioBUnits += increment;
        parentSystem->updateInsulinLevel(increment);

        if(delivered >= units) {
            bolusTimer->deleteLater();
        }
    });

    bolusTimer->start(1000); // 1-second intervals
}

float InsulinDelivery::calculateBolus(float glucose, float carbs) const
{
    const Profile* profile = parentSystem->getCurrentProfile();
    const float target = profile->getTargetGlucose();
    const float correction = (glucose > target) ?
                           (glucose - target) / profile->getCorrectionFactor() : 0.0f;

    return qBound(0.0f,
                (carbs / profile->getCarbRatio()) + correction - ioBUnits,
                25.0f);
}

// Getters
float InsulinDelivery::getCurrentBasalRate() const {
    return currentBasalRate;
}

float InsulinDelivery::getActiveIOB() const {
    return ioBUnits;
}
