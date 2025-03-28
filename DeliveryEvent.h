#ifndef DELIVERYEVENT_H
#define DELIVERYEVENT_H

#include <QDateTime>

class DeliveryEvent{

public:
    enum EventType {BASAL, BOLUS, SUSPEND};

    DeliveryEvent(EventType type, float units);

    //getters
    QDateTime gettimeStamp() const;
    EventType gettype() const;
    float getunits() const;

private:
    QDateTime timeStamp;
    EventType type;
    float units;

};

#endif // DELIVERYEVENT_H
