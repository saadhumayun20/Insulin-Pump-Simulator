#ifndef INSULINDELIVERY_H
#define INSULINDELIVERY_H

#include <QObject>
#include "DeliveryEvent.h"
#include <QTimer>
#include <QtMath>

class PumpSystem;

class InsulinDelivery : public QObject {
    Q_OBJECT

public:
    explicit InsulinDelivery(PumpSystem* parentSystem);

    virtual void startBasal(float baseRate);
    virtual void stopBasal();
    virtual void deliverBolus(float units);
    void deliverExtendedBolus(float totalUnits, float immediateFraction, int durationHours);
    float calculateBolus(float glucose, float carbs) const;
    void quickBolus();

    //getter
    float getCurrentBasalRate() const;
    float getActiveIOB() const;

signals:
    void extendedBolusProgress(float delivered, float remaining);

private:
    struct ExtendedBolus{
        float total;
        float remaining;
        int durationLeft;
    };

    QList<ExtendedBolus> activeExtendedBoluses;
     PumpSystem* parentSystem;
    float currentBasalRate;
    float ioBUnits;

};

#endif // INSULINDELIVERY_H
