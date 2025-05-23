#ifndef CGM_H
#define CGM_H

#include <QObject>
#include <QDateTime>

class CGM : public QObject {
    Q_OBJECT
public:
    explicit CGM(QObject* parent = nullptr);

    virtual float readGlucose() const;
    virtual float getTrend() const;
    void updateGlucose(float value);

signals:
    void glucoseUpdated(float value);
    void pairedStatusChanged(bool paired);

public slots:
    void pairDevice();

private:
    float currentGlucose;
    float previousGlucose;
    QDateTime currentReadingTime;
    QDateTime previousReadingTime;
    bool isPaired = false;

};

#endif // CGM_H
