#ifndef DELIVERYEVENT_H
#define DELIVERYEVENT_H

#include <QDateTime>

class DeliveryEvent{

public:
    enum EventType {BASAL, BOLUS, SUSPEND, EXTENDED_BOLUS, OCCLUSION_ALARMS, BASAL_PAUSED, BASAL_RESUMED, PRIME};

    DeliveryEvent(EventType type, float units);

    //getters
    QDateTime getTimeStamp() const;
    EventType getType() const;
    float getUnits() const;

private:
    QDateTime timeStamp;
    EventType type;
    float units;

};

#endif // DELIVERYEVENT_H
