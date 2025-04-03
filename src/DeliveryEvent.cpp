#include "DeliveryEvent.h"

DeliveryEvent::DeliveryEvent(EventType type, float units) : timeStamp(QDateTime::currentDateTime()),
                                                                      type(type), units(units) {}

QDateTime DeliveryEvent::getTimeStamp() const {return timeStamp; }
DeliveryEvent::EventType DeliveryEvent::getType() const {return type; }
float DeliveryEvent::getUnits() const {return units; }
