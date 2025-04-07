#include "DataLogger.h"

DataLogger::DataLogger(QObject* parent) : QObject(parent) {}

void DataLogger::logDeliveryEvent(const DeliveryEvent &event) {
    deliveryHistory.append(event);
//    deliveryHistory.append(DeliveryEvent(DeliveryEvent::OCCLUSION_ALARMS, 0.0f));
}

QList<DeliveryEvent> DataLogger::getDeliveryHistory() const{
    return deliveryHistory;
}

