#ifndef INSULINDELIVERY_H
#define INSULINDELIVERY_H

#include <QObject>
#include "DeliveryEvent.h"

class PumpSystem;

class InsulinDelivery : public QObject {
    Q_OBJECT

public:
    explicit InsulinDelivery(PumpSystem* parentSystem);

    void startBasal();
    void stopBasal();
    void deliverBolus(float units);
    float calculateBolus(float glucose, float carbs) const;

    //getter
    float getCurrentBasalRate() const;
    float getActiveIOB() const;

private:
    PumpSystem* parentSystem;
    float currentBasalRate;
    float ioBUnits;

};

#endif // INSULINDELIVERY_H
