#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QList>
#include "DeliveryEvent.h"

class DataLogger : public QObject{
    Q_OBJECT

public:
    explicit DataLogger(QObject *parent = nullptr);

    void logDeliveryEvent(const DeliveryEvent& event);
    QList<DeliveryEvent> viewDeliveryHistory() const;
    void generateDataGraphs();

private:
    QList<DeliveryEvent> deliveryHistory;
};

#endif // DATALOGGER_H
