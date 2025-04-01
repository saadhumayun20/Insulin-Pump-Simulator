#include "ControlIQ.h"
#include "PumpSystem.h"
#include <QtMath>

// Constants (match Tandem specs)
const float TARGET_LOW = 112.5f;    // 6.3 mmol/L
const float TARGET_HIGH = 160.0f;   // 8.9 mmol/L
const float HYPO_THRESHOLD = 70.0f;
const float HYPER_THRESHOLD = 180.0f;
const float MAX_AUTO_BOLUS = 6.0f;
const int PREDICTION_MINUTES = 30;

ControlIQ::ControlIQ(CGM* cgm, QObject *parent)
    : QObject(parent), cgm(cgm)
{
    QTimer* adjustmentTimer = new QTimer(this);
    connect(adjustmentTimer, &QTimer::timeout, this, &ControlIQ::adjustBasalRate);
    adjustmentTimer->start(300000); // 5 minutes
}

CGM* ControlIQ::getGlucoseMonitor() const {return cgm; }

void ControlIQ::adjustBasalRate() {
    if(!isEnabled) return;

    PumpSystem* pump = qobject_cast<PumpSystem*>(parent());
    if(!pump) return;

    Profile* profile = pump->getCurrentProfile();  // Get profile from parent
    if(!profile) {return;}
    float currentGlucose = cgm->readGlucose();
    float trend = cgm->getTrend();

    float predictedGlucose;
    predictGlucose(currentGlucose, trend, predictedGlucose);

    if(predictedGlucose <= HYPO_THRESHOLD) {
        autoSuspend();
    }
    else if(predictedGlucose > HYPER_THRESHOLD) {
        deliverAutoCorrection(predictedGlucose);
    }
    else if(predictedGlucose > TARGET_HIGH) {
        float multiplier = qMin(2.0f, 1.0f + (predictedGlucose - TARGET_HIGH)/40.0f);
        pump->getDeliverySystem()->startBasal(profile->getBasalRate() * multiplier);
    }
    else if(predictedGlucose < TARGET_LOW) {
        float multiplier = qMax(0.0f, 1.0f - (TARGET_LOW - predictedGlucose)/40.0f);
        pump->getDeliverySystem()->startBasal(profile->getBasalRate() * multiplier);
    }
}

void ControlIQ::autoSuspend() {
    PumpSystem* pump = qobject_cast<PumpSystem*>(parent());
    if(pump) {
        pump->getDeliverySystem()->stopBasal();
        pump->getLogger()->logDeliveryEvent(DeliveryEvent(DeliveryEvent::SUSPEND, 0.0f));
    }
}

// Private
void ControlIQ::predictGlucose(float current, float trend, float& prediction) {
    prediction = current + trend * PREDICTION_MINUTES;
}

void ControlIQ::deliverAutoCorrection(float predictedGlucose) {
    PumpSystem* pump = qobject_cast<PumpSystem*>(parent());
    if(!pump || lastCorrectionTime.secsTo(QDateTime::currentDateTime()) < 3600) return;

    Profile* profile = pump->getCurrentProfile();
    float correctionDose = (predictedGlucose - TARGET_HIGH) / profile->getCorrectionFactor();

    // Apply 60% correction with safety limits
    float autoBolus = qBound(0.0f, correctionDose * 0.6f, MAX_AUTO_BOLUS);

    if(autoBolus > 0) {
        pump->getDeliverySystem()->deliverBolus(autoBolus);
        lastCorrectionTime = QDateTime::currentDateTime();
    }
}
