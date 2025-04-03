#ifndef CGM_H
#define CGM_H

#include <QObject>
#include <QDateTime>

class CGM : public QObject {
    Q_OBJECT
public:
    explicit CGM(QObject* parent = nullptr);

    float readGlucose() const;
    float getTrend() const;  // New
    void updateGlucose(float value);  // For simulation

signals:
    void glucoseUpdated(float value);

private:
    float currentGlucose;
    float previousGlucose;
    QDateTime lastReadingTime;
};

#endif // CGM_H
