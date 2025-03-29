#include "CGM.h"
#include <QDateTime>

CGM::CGM(QObject* parent) : QObject(parent), currentGlucose(0), previousGlucose(0), lastReadingTime(QDateTime::currentDateTime()) {}

float CGM::readGlucose() const {
    return currentGlucose;
}

float CGM::getTrend() const {
    const float minutes = lastReadingTime.secsTo(QDateTime::currentDateTime()) / 60.0f;
    if(minutes <= 0) return 0;
    return (currentGlucose - previousGlucose) / minutes; // mg/dL per minute
}

void CGM::updateGlucose(float value) {
    previousGlucose = currentGlucose;
    currentGlucose = value;
    lastReadingTime = QDateTime::currentDateTime();
    emit glucoseUpdated(value);
}
