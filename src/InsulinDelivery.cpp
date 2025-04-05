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

void InsulinDelivery::startBasal(float baseRate)
{
    if(parentSystem->isLocked()) return;

    currentBasalRate = baseRate;
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
    if (parentSystem->isOcclusionActive()) {
        parentSystem->getAlerts()->triggerOcclusionAlarm();
        return;
    }

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

float InsulinDelivery::calculateBolus(float glucose, float carbs) const {
    const Profile* profile = parentSystem->getCurrentProfile();
    if (!profile) { return 0.0f; }

    // Food bolus
    float carbBolus = carbs / profile->getCarbRatio();

    // Correction bolus
    float correction = (glucose > profile->getTargetGlucose())
        ? (glucose - profile->getTargetGlucose()) / profile->getCorrectionFactor()
        : 0.0f;

    // Total with IOB adjustment
    return qBound(0.0f, carbBolus + correction - ioBUnits, 25.0f);
}

// Extended bolus delivery system
void InsulinDelivery::deliverExtendedBolus(float totalUnits,
                                          float immediateFraction,
                                          int durationHours) {
    if(totalUnits <= 0 || durationHours < 1) return;

    // Immediate portion
    float immediateUnits = totalUnits * immediateFraction;
    if(immediateUnits > 0) {
        deliverBolus(immediateUnits); // Use existing bolus delivery
    }

    // Extended portion
    float extendedUnits = totalUnits - immediateUnits;
    ExtendedBolus newBolus{
        .total = extendedUnits,
        .remaining = extendedUnits,
        .durationLeft = durationHours
    };
    activeExtendedBoluses.append(newBolus);

    // Hourly delivery timer
    QTimer* extendedTimer = new QTimer(this);
    connect(extendedTimer, &QTimer::timeout, [=]() mutable {
        for(auto& bolus : activeExtendedBoluses) {
            if(bolus.durationLeft <= 0) continue;

            float hourlyAmount = bolus.total / bolus.durationLeft;
            deliverBolus(hourlyAmount); // Use base delivery method

            bolus.remaining -= hourlyAmount;
            bolus.durationLeft--;

            parentSystem->getLogger()->logDeliveryEvent(
                DeliveryEvent(DeliveryEvent::EXTENDED_BOLUS, hourlyAmount)
            );

            emit extendedBolusProgress(
                bolus.total - bolus.remaining,
                bolus.remaining
            );
        }
    });
    extendedTimer->start(3600000);
}

// Getters
float InsulinDelivery::getCurrentBasalRate() const {
    return currentBasalRate;
}

float InsulinDelivery::getActiveIOB() const {
    return ioBUnits;
}
