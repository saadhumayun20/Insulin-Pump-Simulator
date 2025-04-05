#include "CGM.h"
#include <QDateTime>


CGM::CGM(QObject* parent)
    : QObject(parent),
      currentGlucose(0),
      previousGlucose(0),
      currentReadingTime(QDateTime::currentDateTime()),
      previousReadingTime(currentReadingTime) {}


float CGM::readGlucose() const {
    return currentGlucose;
}

float CGM::getTrend() const {
    float minutes = previousReadingTime.secsTo(currentReadingTime) / 60.0f;
    if (minutes <= 0) return 0;
    return (currentGlucose - previousGlucose) / minutes;
}

void CGM::updateGlucose(float value) {
    previousGlucose = currentGlucose;
    previousReadingTime = currentReadingTime;
    currentGlucose = value;
    currentReadingTime = QDateTime::currentDateTime();
    emit glucoseUpdated(value);
}
