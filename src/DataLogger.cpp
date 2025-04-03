#include "DataLogger.h"

DataLogger::DataLogger(QObject* parent) : QObject(parent) {}

void DataLogger::logDeliveryEvent(const DeliveryEvent &event) {
    deliveryHistory.append(event);
}

QList<DeliveryEvent> DataLogger::getDeliveryHistory() const{
    return deliveryHistory;
}

void DataLogger::generateDataGraphs(){
    //something here?
}
