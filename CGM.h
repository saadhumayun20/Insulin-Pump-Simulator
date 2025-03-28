#ifndef CGM_H
#define CGM_H

#include <QObject>

class CGM : public QObject{
    Q_OBJECT

public:
    explicit CGM(QObject *parent = nullptr);

    bool pairDevice();

    bool isConnected() const; //getter
    float readGlucose() const; //getter


signals:
    void glucoseUpdated(float value);

private:
    bool isConnected();
    float currentGlucose();

};

#endif // CGM_H
